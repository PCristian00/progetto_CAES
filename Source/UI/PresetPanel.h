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

			const auto container = getLocalBounds().reduced(4);
			// const auto container = utils::getBoundsWithPadding(this, 4);
			auto bounds = container;

			// juce::Rectangle<int>

			// auto buttonSize = (bounds.removeFromLeft(container.proportionOfWidth(0.2f)).reduced(4));

			// Rendere migliori queste funzioni

			setButtonBounds(saveButton, bounds.removeFromLeft(container.proportionOfWidth(0.2f)).reduced(4));
			setButtonBounds(previousPresetButton, bounds.removeFromLeft(container.proportionOfWidth(0.1f)).reduced(4));
			presetList.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.4f)).reduced(4));
			setButtonBounds(nextPresetButton, bounds.removeFromLeft(container.proportionOfWidth(0.1f)).reduced(4));
			setButtonBounds(deleteButton, bounds.reduced(4));
			// Set bounds for presetList ComboBox

			// FORSE RIUSARE quella di Utils?
			/*auto comboBoxWidth = static_cast<int>(container.proportionOfWidth(0.2f));
			auto comboBoxBounds = container.removeFromLeft(comboBoxWidth).reduced(4);
			presetList.setBounds(comboBoxBounds);*/




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

		// PER ORA NON UTILIZZATA
		void setButtonBounds(juce::Button& button, juce::Rectangle<int> size) {

			// button.setBounds(x, y, size.getWidth(), size.getHeight());
			button.setBounds(size);
		}

		juce::TextButton saveButton, deleteButton, previousPresetButton, nextPresetButton;
		juce::ComboBox presetList;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetPanel)
	};
} // namespace Gui forse da rimuovere?