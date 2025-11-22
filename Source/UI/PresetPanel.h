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
#include "DialogBox.h"

namespace Gui
{
	class PresetPanel : public juce::Component, public juce::Button::Listener, public juce::ComboBox::Listener
	{
	public:
		PresetPanel(Service::PresetManager& pm) : presetManager(pm) {
			configureButton(saveButton, "Save");
			configureButton(deleteButton, "Delete");
			configureButton(previousPresetButton, "<");
			configureButton(nextPresetButton, ">");

			// Configurazione presetList ComboBox
			presetList.setTextWhenNothingSelected("Select Preset");
			presetList.setMouseCursor(juce::MouseCursor::PointingHandCursor);
			addAndMakeVisible(presetList);
			presetList.addListener(this);

			loadPresetList();
		}

		~PresetPanel() {
			saveButton.removeListener(this);
			deleteButton.removeListener(this);
			previousPresetButton.removeListener(this);
			nextPresetButton.removeListener(this);
			presetList.removeListener(this);
		}

		void resized() override {

			const auto container = getLocalBounds().reduced(4);
			auto bounds = container;

			// Rendere migliori queste funzioni

			setButtonBounds(saveButton, bounds.removeFromLeft(container.proportionOfWidth(0.2f)).reduced(4));
			setButtonBounds(previousPresetButton, bounds.removeFromLeft(container.proportionOfWidth(0.1f)).reduced(4));
			presetList.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.4f)).reduced(4));
			setButtonBounds(nextPresetButton, bounds.removeFromLeft(container.proportionOfWidth(0.1f)).reduced(4));
			setButtonBounds(deleteButton, bounds.reduced(4));
		}

	private:

		void loadPresetList() {

			presetList.clear(juce::dontSendNotification);

			const auto allPresets = presetManager.getAllPresets();
			const auto currentPreset = presetManager.getCurrentPreset();
			presetList.addItemList(allPresets, 1);
			presetList.setSelectedItemIndex(allPresets.indexOf(currentPreset), juce::dontSendNotification);
		}

		void buttonClicked(juce::Button* button) override {

			if (button == &saveButton) {
				fileChooser = std::make_unique<juce::FileChooser>("Save Preset", Service::PresetManager::defaultDirectory, "*." + Service::PresetManager::extension);
				fileChooser->launchAsync(juce::FileBrowserComponent::saveMode, [&](const juce::FileChooser& chooser) {
					auto file = chooser.getResult();
					auto presetName = file.getFileNameWithoutExtension();

					if (presetManager.isEmbeddedPreset(presetName)) {

						juce::String msg = "Il nome preset '" + presetManager.getCurrentPreset() + "' è usato da un preset default. Cambiare nome.";

						std::function <void()> onAccept = [this]() {dialogBox.reset();};

						showDialogBox(msg, "", "Ok ok", onAccept);
						return;
					}

					presetManager.savePreset(presetName);
					loadPresetList();
					});
			}
			else if (button == &deleteButton)
			{

				if (presetManager.getCurrentPreset().isEmpty())
					return;

				if (presetManager.isEmbeddedPreset(presetManager.getCurrentPreset()))
					return;


				juce::String msg = "Cancellare il preset '" + presetManager.getCurrentPreset() + "' ? ";

				std::function <void()> onAccept = [this]() {
					presetManager.deletePreset(presetManager.getCurrentPreset());
					loadPresetList();
					dialogBox.reset();
					};

				showDialogBox(msg, "Cancella preset", "Annulla", onAccept);
			}
			else if (button == &previousPresetButton) {
				presetList.setSelectedItemIndex(presetManager.loadPreviousPreset(), juce::dontSendNotification);
			}
			else if (button == &nextPresetButton) {
				presetList.setSelectedItemIndex(presetManager.loadNextPreset(), juce::dontSendNotification);
			}
		}

		void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override {
			if (comboBoxThatHasChanged == &presetList) {
				auto selectedPreset = presetList.getItemText(presetList.getSelectedItemIndex());
				presetManager.loadPreset(selectedPreset);
			}
		}

		void configureButton(juce::Button& button, const juce::String& buttonText) {
			button.setButtonText(buttonText);
			button.setMouseCursor(juce::MouseCursor::PointingHandCursor);
			addAndMakeVisible(button);
			button.addListener(this);
		}

		// da migliorare, spostare in utils o rimuovere
		void setButtonBounds(juce::Button& button, juce::Rectangle<int> size) {
			button.setBounds(size);
		}

		void showDialogBox(juce::String msg, juce::String confirmText, juce::String returnText, std::function<void()> onAccept) {

			dialogBox = std::make_unique<DialogBox>(msg, confirmText, returnText, onAccept);

			addAndMakeVisible(*dialogBox);
			dialogBox->setBounds(0, 0, getWidth(), getHeight());

		}

		Service::PresetManager& presetManager;
		std::unique_ptr<DialogBox> dialogBox;
		juce::TextButton saveButton, deleteButton, previousPresetButton, nextPresetButton;
		juce::ComboBox presetList;
		std::unique_ptr<juce::FileChooser> fileChooser;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetPanel)
	};
} // namespace Gui