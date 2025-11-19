/*
  ==============================================================================

	PresetManager.cpp
	Created: 19 Nov 2025 2:39:14pm
	Author:  crist

  ==============================================================================
*/

#include "PresetManager.h"

namespace Service {

	const File PresetManager::defaultDirectory{
		File::getSpecialLocation(File::commonDocumentsDirectory)
			.getChildFile(ProjectInfo::companyName)
			.getChildFile(ProjectInfo::projectName)
	};

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
	}

	PresetManager::~PresetManager()
	{
		valueTreeState.state.removeListener(this);
	}

	void PresetManager::savePreset(const juce::String& presetName)
	{
		if (presetName.isEmpty())
			return;

		currentPreset.setValue(presetName);

		const auto xml = valueTreeState.copyState().createXml();
		const auto presetFile = defaultDirectory.getChildFile(presetName + "." + extension);

		if (!xml->writeTo(presetFile))
		{
			DBG("Failed to save preset: " + presetFile.getFullPathName());
			jassertfalse;
		}
	}

	void PresetManager::deletePreset(const String& presetName)
	{
		if (presetName.isEmpty())
			return;

		const auto presetFile = defaultDirectory.getChildFile(presetName + "." + extension);

		if (!presetFile.existsAsFile())
		{
			DBG("Preset file does not exist: " + presetFile.getFullPathName());
			jassertfalse;
			return;
		}

		if (!presetFile.deleteFile())
		{
			DBG("Failed to delete preset: " + presetFile.getFullPathName());
			jassertfalse;
			return;
		}

		currentPreset.setValue("");
	}

	void PresetManager::loadPreset(const juce::String& presetName)
	{
		if (presetName.isEmpty())
			return;

		const auto presetFile = defaultDirectory.getChildFile(presetName + "." + extension);
		if (!presetFile.existsAsFile())
		{
			DBG("Preset file does not exist: " + presetFile.getFullPathName());
			jassertfalse;
			return;
		}

		juce::XmlDocument xmlDoc{ presetFile };
		const auto valueTreeToLoad = juce::ValueTree::fromXml(*xmlDoc.getDocumentElement());

		valueTreeState.replaceState(valueTreeToLoad);
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

	juce::StringArray PresetManager::getAllPresets() const
	{
		juce::StringArray presets;
		// Aggiunto il punto prima dell'estensione
		const auto fileArray = defaultDirectory.findChildFiles(juce::File::findFiles, false, "*." + extension);

		for (const auto& file : fileArray)
			presets.add(file.getFileNameWithoutExtension());

		return presets;
	}

	juce::String PresetManager::getCurrentPreset() const
	{
		return currentPreset.toString();
	}

	void PresetManager::valueTreeRedirected(juce::ValueTree& treeWhichHasBeenChanged)
	{
		currentPreset.referTo(treeWhichHasBeenChanged.getPropertyAsValue(presetNameProperty, nullptr));
	}
} // namespace Service