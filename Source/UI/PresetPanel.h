/*
  ==============================================================================

	PresetPanel.h
	Created: 19 Nov 2025 11:54:28am
	Author:  crist

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Utils.h"

namespace Gui
{
	class PresetPanel : public juce::Component, public juce::Button::Listener, public juce::ComboBox::Listener
	{
	public:
		PresetPanel() {
			configureButton(saveButton, "Save");
			configureButton(deleteButton, "Delete");
			configureButton(previousPresetButton, "<");
			configureButton(nextPresetButton, ">");

			// Configure presetList ComboBox here
			presetList.setTextWhenNothingSelected("Select Preset");
			presetList.setMouseCursor(juce::MouseCursor::PointingHandCursor);
			addAndMakeVisible(presetList);
			presetList.addListener(this);

		}

		~PresetPanel() {
			saveButton.removeListener(this);
			deleteButton.removeListener(this);
			previousPresetButton.removeListener(this);
			nextPresetButton.removeListener(this);

			presetList.removeListener(this);
		}

		// void paint(juce::Graphics& g) override;

		void resized() override {

			auto container = getLocalBounds().reduced(4);

			setButtonBounds(saveButton, 0.2f);
			// setButtonBounds(deleteButton, 0.2f);
			deleteButton.setBounds(container.reduced(4));
			setButtonBounds(previousPresetButton, 0.1f);
			setButtonBounds(nextPresetButton, 0.1f);
			// Set bounds for presetList ComboBox

			// FORSE RIUSARE quella di Utils?
			auto comboBoxWidth = static_cast<int>(container.proportionOfWidth(0.2f));
			auto comboBoxBounds = container.removeFromLeft(comboBoxWidth).reduced(4);
			presetList.setBounds(comboBoxBounds);




		}
	private:

		// FORSE IN FUTURO SERVIRANNO ANCHE ALTROVE, SPOSTARE IN UTILS?

		void buttonClicked(juce::Button* button) override {

		}

		void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override {
		}

		void configureButton(juce::Button& button, const juce::String& buttonText) {
			// Configure buttons here
			button.setButtonText(buttonText);
			button.setMouseCursor(juce::MouseCursor::PointingHandCursor);
			addAndMakeVisible(button);
			button.addListener(this);
		}

		void setButtonBounds(juce::Button& button, float proportion) {

			// const auto container = utils::getBoundsWithPadding(this, 4);
			/*const auto container = getLocalBounds().reduced(4);
			const auto bounds = container;

			button.setBounds(bounds.removeFromLeft(container.proportionOfWidth(proportion).reduced(4)));*/

			auto container = getLocalBounds().reduced(4);
			auto width = static_cast<int>(container.proportionOfWidth(proportion));
			auto buttonBounds = container.removeFromLeft(width).reduced(4);
			button.setBounds(buttonBounds);
		}

		juce::TextButton saveButton, deleteButton, previousPresetButton, nextPresetButton;
		juce::ComboBox presetList;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetPanel)
	};
} // namespace Gui forse da rimuovere?