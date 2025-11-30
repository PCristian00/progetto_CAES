/*
  ==============================================================================

	PresetPanel.h
	Created: 19 Nov 2025 11:54:28am
	Author:  crist

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "../Service/PresetManager.h"
#include "DialogBox.h"

namespace Gui
{
	class PresetPanel : public juce::Component, public juce::Button::Listener, public juce::ComboBox::Listener
	{
	public:
		PresetPanel(Service::PresetManager& pm);
		~PresetPanel() override;

		void paint(juce::Graphics& g) override;
		void resized() override;

	private:
		void loadPresetList();

		void buttonClicked(juce::Button* button) override;
		void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;

		void checkPreset(juce::String preset);
		void showDialogBox(juce::String msg, juce::String confirmText, juce::String returnText, std::function<void()> onAccept);

		Service::PresetManager& presetManager;
		std::unique_ptr<DialogBox> dialogBox;
		juce::TextButton saveButton, deleteButton, previousPresetButton, nextPresetButton;
		juce::ComboBox presetList;
		std::unique_ptr<juce::FileChooser> fileChooser;
		juce::Colour defaultListTextColour;
		juce::Colour defaultListBgColour;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetPanel)
	};
}