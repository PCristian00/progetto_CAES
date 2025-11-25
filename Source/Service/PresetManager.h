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
		~PresetManager();

		void savePreset(const String& presetName);
		void deletePreset(const String& presetName);
		void loadPreset(const String& presetName);

		int loadNextPreset();
		int loadPreviousPreset();

		juce::StringArray getAllPresets() const;
		juce::StringArray getUserPresets() const;
		juce::StringArray getEmbeddedPresets() const;

		String getCurrentPreset() const;

		bool isEmbeddedPreset(const String& presetName) const;
		bool isValidUserPresetName(const String& presetName) const;

	private:
		void valueTreeRedirected(juce::ValueTree& treeWhichHasBeenChanged) override;

		void loadEmbeddedPreset(const String& presetName);
		juce::ValueTree valueTreeFromEmbeddedXml(const void* data, size_t size) const;

		// Rimuove dallo stato eventuali parametri "orfani" non presenti nell'APVTS
		void purgeUnknownParameters();

		juce::AudioProcessorValueTreeState& valueTreeState;
		juce::Value currentPreset;
	};
} // namespace Service