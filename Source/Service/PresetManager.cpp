/*
  ==============================================================================

	PresetManager.cpp
	Created: 19 Nov 2025 2:39:14pm
	Author:  crist

  ==============================================================================
*/

#include "PresetManager.h"

namespace Service {

	const File PresetManager::defaultDirectory{ File::getSpecialLocation(File::commonDocumentsDirectory).getChildFile(ProjectInfo::companyName).getChildFile(ProjectInfo::projectName) };

	const String PresetManager::extension{ "preset" };
	const String PresetManager::presetNameProperty{ "presetName" };

	PresetManager::PresetManager(juce::AudioProcessorValueTreeState& apvts)
		: valueTreeState(apvts)
	{
		if (!defaultDirectory.exists())
		{
			const juce::Result result = defaultDirectory.createDirectory();
			if (result.failed())
			{
				DBG("Failed to create presets directory: " + result.getErrorMessage());
				jassertfalse;
			}
		}

		valueTreeState.state.addListener(this);

		// Garantisce che la property esista
		if (!valueTreeState.state.hasProperty(presetNameProperty))
			valueTreeState.state.setProperty(presetNameProperty, "", nullptr);

		currentPreset.referTo(valueTreeState.state.getPropertyAsValue(presetNameProperty, nullptr));

		// Pulisce eventuali parametri orfani ancora presenti nello stato
		purgeUnknownParameters();
	}

	PresetManager::~PresetManager()
	{
		valueTreeState.state.removeListener(this);
	}

	static void writeValueTreeToFile(const juce::ValueTree& vt, const File& file)
	{
		auto copy = vt;
		if (!copy.hasProperty("version"))
			copy.setProperty("version", ProjectInfo::versionString, nullptr);

		auto xml = copy.createXml();
		if (!xml->writeTo(file))
		{
			DBG("Failed to write preset: " + file.getFullPathName());
			jassertfalse;
		}
	}

	bool PresetManager::isValidUserPresetName(const String& presetName) const
	{
		if (presetName.isEmpty())
			return false;
		// Se esiste un embedded con lo stesso nome blocchiamo
		if (isEmbeddedPreset(presetName))
			return false;
		return true;
	}

	// Consenti solo id e value
	static bool isAllowedParamProp(const juce::Identifier& name) noexcept
	{
		return name == juce::Identifier("id") || name == juce::Identifier("value");
	}

	static void stripParamNodeToIdAndValue(juce::ValueTree& node) noexcept
	{
		for (int i = node.getNumProperties() - 1; i >= 0; --i)
		{
			auto name = node.getPropertyName(i);
			if (!isAllowedParamProp(name))
				node.removeProperty(name, nullptr);
		}
	}

	// Mantiene i valori "reali" nel tree e rimuove metadata extra
	static void sanitizeParamNodesForSave(juce::ValueTree& treeRoot) noexcept
	{
		for (int i = 0; i < treeRoot.getNumChildren(); ++i)
		{
			auto child = treeRoot.getChild(i);

			// Assicura che "value" rimanga numerico (non stringa)
			if (child.hasProperty("value"))
			{
				const auto v = child.getProperty("value");
				if (v.isString())
				{
					// Prova a convertirlo a numero se possibile
					const auto s = v.toString();
					const double num = s.getDoubleValue();
					child.setProperty("value", num, nullptr);
				}
			}

			stripParamNodeToIdAndValue(child);
		}
	}

	// Rileva se un valore salvato probabilmente è normalizzato [0..1] mentre il range del parametro non lo è
	static bool looksLikeNormalizedForParam(const juce::RangedAudioParameter* p, double v) noexcept
	{
		if (p == nullptr)
			return false;

		const auto range = p->getNormalisableRange();
		const bool in01 = (v >= 0.0 && v <= 1.0);
		const bool paramNotUnitRange = (range.start != 0.0f || range.end != 1.0f);
		return in01 && paramNotUnitRange;
	}

	void PresetManager::savePreset(const juce::String& presetName)
	{
		if (!isValidUserPresetName(presetName))
		{
			DBG("Nome preset non valido o riservato (factory): " + presetName);
			return;
		}

		// Pulisce orfani
		purgeUnknownParameters();

		currentPreset.setValue(presetName);

		const auto presetFile = defaultDirectory.getChildFile(presetName + "." + extension);

		// Copia dello stato mantenendo i valori reali e riducendo ai soli id/value
		auto treeToWrite = valueTreeState.copyState();
		sanitizeParamNodesForSave(treeToWrite);

		// Scrittura
		writeValueTreeToFile(treeToWrite, presetFile);
	}

	bool PresetManager::isEmbeddedPreset(const String& presetName) const
	{
		if (presetName.isEmpty())
			return false;

		for (int i = 0; i < BinaryData::namedResourceListSize; ++i)
		{
			const String originalName = BinaryData::originalFilenames[i];
			if (originalName.endsWith("." + extension))
			{
				auto base = originalName.dropLastCharacters(extension.length() + 1);
				if (base == presetName)
					return true;
			}
		}
		return false;
	}

	juce::ValueTree PresetManager::valueTreeFromEmbeddedXml(const void* data, size_t size) const
	{
		if (data == nullptr || size == 0)
			return {};

		const juce::String xmlString = juce::String::fromUTF8(static_cast<const char*>(data), (int)size);
		std::unique_ptr<juce::XmlElement> xml(juce::parseXML(xmlString));
		if (!xml)
		{
			DBG("Embedded preset XML parse failed.");
			return {};
		}
		return juce::ValueTree::fromXml(*xml);
	}

	void PresetManager::loadEmbeddedPreset(const String& presetName)
	{
		for (int i = 0; i < BinaryData::namedResourceListSize; ++i)
		{
			const String originalName = BinaryData::originalFilenames[i];
			if (!originalName.endsWith("." + extension))
				continue;

			auto base = originalName.dropLastCharacters(extension.length() + 1);
			if (base != presetName)
				continue;

			int dataSize = 0;
			const char* data = BinaryData::getNamedResource(BinaryData::namedResourceList[i], dataSize);
			if (data == nullptr || dataSize <= 0)
			{
				DBG("Embedded resource pointer invalid for preset: " + presetName);
				jassertfalse;
				return;
			}

			auto vt = valueTreeFromEmbeddedXml(data, (size_t)dataSize);
			if (!vt.isValid())
			{
				DBG("Embedded preset ValueTree invalid: " + presetName);
				jassertfalse;
				return;
			}

			for (int c = 0; c < vt.getNumChildren(); ++c)
			{
				const auto paramChild = vt.getChild(c);
				const auto paramID = paramChild.getProperty("id");
				auto paramTree = valueTreeState.state.getChildWithProperty("id", paramID);

				if (!paramTree.isValid())
					continue;

				// Migrazione: se "value" sembra normalizzato [0..1] e il parametro non ha range unitario, converti a valore reale
				if (paramChild.hasProperty("value"))
				{
					const double stored = static_cast<double>(paramChild.getProperty("value"));
					if (auto* p = dynamic_cast<juce::RangedAudioParameter*>(valueTreeState.getParameter(paramID.toString())))
					{
						const double corrected = looksLikeNormalizedForParam(p, stored)
							? static_cast<double>(p->convertFrom0to1(static_cast<float>(stored)))
							: stored;

						paramTree.setProperty("value", corrected, nullptr);
					}
					else
					{
						paramTree.setProperty("value", stored, nullptr);
					}
				}

				// Pulisci il nodo nello stato
				stripParamNodeToIdAndValue(paramTree);
			}

			currentPreset.setValue(presetName);
			return;
		}

		DBG("Embedded preset not found: " + presetName);
		jassertfalse;
	}

	void PresetManager::deletePreset(const String& presetName)
	{
		if (presetName.isEmpty())
			return;

		const auto presetFile = defaultDirectory.getChildFile(presetName + "." + extension);
		if (!presetFile.existsAsFile())
		{
			if (isEmbeddedPreset(presetName))
			{
				DBG("Cannot delete embedded (factory) preset: " + presetName);
				return;
			}
			DBG("Preset file does not exist: " + presetName);
			jassertfalse;
			return;
		}

		if (!presetFile.deleteFile())
		{
			DBG("Failed to delete preset: " + presetFile.getFullPathName());
			jassertfalse;
			return;
		}

		if (currentPreset.toString() == presetName)
			currentPreset.setValue("");
	}

	void PresetManager::loadPreset(const juce::String& presetName)
	{
		if (presetName.isEmpty())
			return;

		if (isEmbeddedPreset(presetName))
		{
			loadEmbeddedPreset(presetName);
			return;
		}

		const auto presetFile = defaultDirectory.getChildFile(presetName + "." + extension);
		if (!presetFile.existsAsFile())
		{
			DBG("Preset file does not exist (user): " + presetName);
			jassertfalse;
			return;
		}

		juce::XmlDocument xmlDoc{ presetFile };
		std::unique_ptr<juce::XmlElement> xml(xmlDoc.getDocumentElement());
		if (!xml)
		{
			DBG("Invalid XML in user preset: " + presetFile.getFullPathName());
			jassertfalse;
			return;
		}

		const auto valueTreeToLoad = juce::ValueTree::fromXml(*xml);

		for (int i = 0; i < valueTreeToLoad.getNumChildren(); i++)
		{
			const auto paramChildToLoad = valueTreeToLoad.getChild(i);
			const auto paramID = paramChildToLoad.getProperty("id");
			auto paramTree = valueTreeState.state.getChildWithProperty("id", paramID);

			if (!paramTree.isValid())
				continue;

			// Migrazione valori salvati normalizzati -> reali
			if (paramChildToLoad.hasProperty("value"))
			{
				const double stored = static_cast<double>(paramChildToLoad.getProperty("value"));
				if (auto* p = dynamic_cast<juce::RangedAudioParameter*>(valueTreeState.getParameter(paramID.toString())))
				{
					const double corrected = looksLikeNormalizedForParam(p, stored)
						? static_cast<double>(p->convertFrom0to1(static_cast<float>(stored)))
						: stored;

					paramTree.setProperty("value", corrected, nullptr);
				}
				else
				{
					paramTree.setProperty("value", stored, nullptr);
				}
			}

			// Ripulisci il nodo nello stato da proprieta' extra
			stripParamNodeToIdAndValue(paramTree);
		}

		currentPreset.setValue(presetName);
	}

	int PresetManager::loadNextPreset()
	{
		const auto presets = getAllPresets();
		if (presets.isEmpty())
			return -1;

		const int currentIndex = presets.indexOf(currentPreset.toString());
		const int nextIndex = (currentIndex + 1 > (presets.size() - 1)) ? 0 : currentIndex + 1;
		loadPreset(presets.getReference(nextIndex));
		return nextIndex;
	}

	int PresetManager::loadPreviousPreset()
	{
		const auto presets = getAllPresets();
		if (presets.isEmpty())
			return -1;

		const int currentIndex = presets.indexOf(currentPreset.toString());
		const int previousIndex = (currentIndex - 1 < 0) ? presets.size() - 1 : currentIndex - 1;
		loadPreset(presets.getReference(previousIndex));
		return previousIndex;
	}

	juce::StringArray PresetManager::getUserPresets() const
	{
		juce::StringArray presets;
		const auto fileArray = defaultDirectory.findChildFiles(juce::File::findFiles, false, "*." + extension);
		for (const auto& file : fileArray)
			presets.add(file.getFileNameWithoutExtension());
		presets.sort(true);
		return presets;
	}

	juce::StringArray PresetManager::getEmbeddedPresets() const
	{
		juce::StringArray names;
		for (int i = 0; i < BinaryData::namedResourceListSize; ++i)
		{
			const String originalName = BinaryData::originalFilenames[i];
			if (originalName.endsWith("." + extension))
			{
				auto base = originalName.dropLastCharacters(extension.length() + 1);
				names.addIfNotAlreadyThere(base);
			}
		}
		names.sort(true);
		return names;
	}

	juce::StringArray PresetManager::getAllPresets() const
	{
		auto embedded = getEmbeddedPresets();
		auto user = getUserPresets();
		juce::StringArray combined;
		combined.addArray(embedded);
		combined.addArray(user);
		return combined;
	}

	juce::String PresetManager::getCurrentPreset() const
	{
		return currentPreset.toString();
	}

	void PresetManager::valueTreeRedirected(juce::ValueTree& treeWhichHasBeenChanged)
	{
		currentPreset.referTo(treeWhichHasBeenChanged.getPropertyAsValue(presetNameProperty, nullptr));

		// Se lo stato viene reindirizzato/sostituito, puliamolo da parametri orfani
		purgeUnknownParameters();
	}

	void PresetManager::purgeUnknownParameters()
	{
		juce::Array<juce::ValueTree> toRemove;

		for (int i = 0; i < valueTreeState.state.getNumChildren(); ++i)
		{
			const auto child = valueTreeState.state.getChild(i);

			if (!child.hasProperty("id"))
				continue;

			juce::String id = (child.getProperty("id")).toString();

			// Se l'APVTS non ha un parametro con questo id, il nodo e' orfano
			if (valueTreeState.getParameter(id) == nullptr)
				toRemove.add(child);
		}

		for (const auto& orphan : toRemove)
			valueTreeState.state.removeChild(orphan, nullptr);
	}
} // namespace Service