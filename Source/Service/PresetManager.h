/*
  ==============================================================================

	PresetManager.h
	Created: 19 Nov 2025 2:39:14pm
	Author:  crist

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using File = juce::File;
using String = juce::String;

namespace Service {
	class PresetManager : juce::ValueTree::Listener {
	public:

		static const File defaultDirectory;
		static const String extension;
		static const String presetNameProperty;

		PresetManager(juce::AudioProcessorValueTreeState& apvts);
		~PresetManager() = default;
		void savePreset(const String& presetName);
		void deletePreset(const String& presetName);
		void loadPreset(const String& presetName);

		int loadNextPreset();
		int loadPreviousPreset();

		juce::StringArray getAllPresets() const;
		String getCurrentPreset() const;

	private:
		juce::AudioProcessorValueTreeState& valueTreeState;
		juce::Value currentPreset;

		void valueTreeRedirected(juce::ValueTree& treeWhichHasBeenChanged) override;
		/*juce::File presetsDirectory;
		juce::StringArray presetNames;
		int currentPresetIndex = -1;
		void refreshPresetList();*/
	};
} // namespace Service