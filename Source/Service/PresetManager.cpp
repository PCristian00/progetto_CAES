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

	/**
	 * Costruttore: prepara directory preset utente, collega il ValueTree dell'APVTS
	 * e garantisce la presenza della property presetName. Rimuove eventuali nodi
	 * parametri orfani dallo stato.
	 *
	 * @param apvts riferimento all'AudioProcessorValueTreeState da serializzare.
	 */
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

		if (!valueTreeState.state.hasProperty(presetNameProperty))
			valueTreeState.state.setProperty(presetNameProperty, "", nullptr);

		currentPreset.referTo(valueTreeState.state.getPropertyAsValue(presetNameProperty, nullptr));
		purgeUnknownParameters();
	}

	/**
	 * Distruttore: rimuove il listener dal ValueTree.
	 */
	PresetManager::~PresetManager()
	{
		valueTreeState.state.removeListener(this);
	}

	/**
	 * Scrive un ValueTree su file come XML, aggiungendo una property 'version'
	 * se assente.
	 *
	 * @param vt   ValueTree da serializzare.
	 * @param file file di destinazione (verrà sovrascritto).
	 */
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

	/**
	 * Verifica se un nome preset utente è valido (non vuoto e non confligge
	 * con i factory embedded).
	 *
	 * @param presetName nome da validare.
	 * @return true se utilizzabile.
	 */
	bool PresetManager::isValidUserPresetName(const String& presetName) const
	{
		if (presetName.isEmpty())
			return false;
		if (isEmbeddedPreset(presetName))
			return false;
		return true;
	}

	/**
	 * Salva lo stato corrente dell'APVTS come preset utente con il nome fornito.
	 * Se il nome non è valido, abortisce. Prima rimuove parametri orfani.
	 *
	 * @param presetName nome file (senza estensione).
	 */
	void PresetManager::savePreset(const juce::String& presetName)
	{
		if (!isValidUserPresetName(presetName))
		{
			DBG("Nome preset non valido o riservato (factory): " + presetName);
			return;
		}

		purgeUnknownParameters();
		currentPreset.setValue(presetName);

		const auto presetFile = defaultDirectory.getChildFile(presetName + "." + extension);
		writeValueTreeToFile(valueTreeState.copyState(), presetFile);
	}

	/**
	 * Ritorna true se il nome corrisponde a un preset embedded (factory).
	 *
	 * @param presetName nome da testare.
	 */
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

	/**
	 * Converte risorsa embedded (bytes) in ValueTree.
	 *
	 * @param data puntatore ai dati XML.
	 * @param size lunghezza in byte.
	 * @return ValueTree valido o vuoto se parsing fallisce.
	 */
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

	/**
	 * Carica un preset embedded (factory) copiando le proprietà dei parametri
	 * presenti nel ValueTree sullo stato corrente.
	 *
	 * @param presetName nome del preset factory.
	 */
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
				if (paramTree.isValid())
					paramTree.copyPropertiesFrom(paramChild, nullptr);
			}

			currentPreset.setValue(presetName);
			return;
		}

		DBG("Embedded preset not found: " + presetName);
		jassertfalse;
	}

	/**
	 * Elimina un preset utente (non factory). Se è il corrente azzera currentPreset.
	 *
	 * @param presetName nome da eliminare.
	 */
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

	/**
	 * Carica un preset (utente o factory) copiando le proprietà dei parametri.
	 *
	 * @param presetName nome del preset.
	 */
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

			if (paramTree.isValid())
				paramTree.copyPropertiesFrom(paramChildToLoad, nullptr);
		}

		currentPreset.setValue(presetName);
	}

	/**
	 * Carica il preset successivo nella lista combinata (factory + user).
	 *
	 * @return indice caricato, -1 se non ci sono preset.
	 */
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

	/**
	 * Carica il preset precedente nella lista combinata (factory + user).
	 *
	 * @return indice caricato, -1 se non ci sono preset.
	 */
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

	/**
	 * Ritorna lista ordinata dei preset utente (nomi senza estensione).
	 */
	juce::StringArray PresetManager::getUserPresets() const
	{
		juce::StringArray presets;
		const auto fileArray = defaultDirectory.findChildFiles(juce::File::findFiles, false, "*." + extension);
		for (const auto& file : fileArray)
			presets.add(file.getFileNameWithoutExtension());
		presets.sort(true);
		return presets;
	}

	/**
	 * Ritorna lista ordinata dei preset embedded (factory).
	 */
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

	/**
	 * Ritorna lista combinata (factory + user) senza ordinamento aggiuntivo.
	 */
	juce::StringArray PresetManager::getAllPresets() const
	{
		auto embedded = getEmbeddedPresets();
		auto user = getUserPresets();
		juce::StringArray combined;
		combined.addArray(embedded);
		combined.addArray(user);
		return combined;
	}

	/**
	 * @return nome del preset correntemente caricato (stringa vuota se nessuno).
	 */
	juce::String PresetManager::getCurrentPreset() const
	{
		return currentPreset.toString();
	}

	/**
	 * Listener: chiamato se il ValueTree radice viene "reindirizzato".
	 * Aggiorna il riferimento a presetName e rimuove parametri orfani.
	 *
	 * @param treeWhichHasBeenChanged nuovo ValueTree radice.
	 */
	void PresetManager::valueTreeRedirected(juce::ValueTree& treeWhichHasBeenChanged)
	{
		currentPreset.referTo(treeWhichHasBeenChanged.getPropertyAsValue(presetNameProperty, nullptr));
		purgeUnknownParameters();
	}

	/**
	 * Rimuove dal ValueTree dello stato parametri che non esistono più
	 * nell'APVTS (evita salvataggi incoerenti).
	 */
	void PresetManager::purgeUnknownParameters()
	{
		juce::Array<juce::ValueTree> toRemove;

		for (int i = 0; i < valueTreeState.state.getNumChildren(); ++i)
		{
			const auto child = valueTreeState.state.getChild(i);
			if (!child.hasProperty("id"))
				continue;

			juce::String id = (child.getProperty("id")).toString();
			if (valueTreeState.getParameter(id) == nullptr)
				toRemove.add(child);
		}

		for (const auto& orphan : toRemove)
			valueTreeState.state.removeChild(orphan, nullptr);
	}
} // namespace Service