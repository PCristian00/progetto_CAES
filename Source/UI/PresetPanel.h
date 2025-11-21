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

			// Pulsanti delete window
			configureButton(confirmButton, "si");
			configureButton(returnButton, "no");

			// Configure presetList ComboBox here
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

		void loadPresetList() {

			presetList.clear(juce::dontSendNotification);

			const auto allPresets = presetManager.getAllPresets();
			const auto currentPreset = presetManager.getCurrentPreset();
			presetList.addItemList(allPresets, 1);
			presetList.setSelectedItemIndex(allPresets.indexOf(currentPreset), juce::dontSendNotification);
		}

		void buttonClicked(juce::Button* button) override {
			if (button == &saveButton) {
				// Handle save button click
				fileChooser = std::make_unique<juce::FileChooser>("Save Preset", Service::PresetManager::defaultDirectory, "*." + Service::PresetManager::extension);
				fileChooser->launchAsync(juce::FileBrowserComponent::saveMode, [&](const juce::FileChooser& chooser) {
					auto file = chooser.getResult();
					auto presetName = file.getFileNameWithoutExtension();
					presetManager.savePreset(presetName);
					loadPresetList();

					/*if (file != juce::File()) {
						auto presetName = file.getFileNameWithoutExtension();
						presetManager.savePreset(presetName);
					}*/
					});
			}
			else if (button == &deleteButton)
			{
				const auto currentPresetName = presetManager.getCurrentPreset();

				if (currentPresetName.isEmpty())
					return;



				showDeleteWindow();

				//// Comportamento pulsanti ok/cancel
				//const auto callback = juce::ModalCallbackFunction::create([this, currentPresetName](int result) {
				//	if (result == 0) {

				//	}// result == 0 means you click Cancel
				//	if (result == 1) {
				//		presetManager.deletePreset(currentPresetName);
				//		loadPresetList();
				//	}// result == 1 means you click OK
				//	});

				//juce::AlertWindow::showOkCancelBox(
				//	juce::AlertWindow::WarningIcon,                       // Icona
				//	"Cancella Preset",                                      // Titolo finestra
				//	"Cancellare preset \"" + currentPresetName + "\"?", // Messaggio
				//	"Sì",                                                // Testo pulsante OK
				//	"No",                                                 // Testo pulsante Cancel
				//	this,												// Component associato (per modalità)
				//	callback 										// Nessun callback: chiamata sincrona, ritorna subito il bool
				//);
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

		// Creare componente esternO!!!
		void showDeleteWindow() {


			std::function <void()> onAccept = [this]() {
				presetManager.deletePreset(presetManager.getCurrentPreset());
				loadPresetList();
				deleteDialog.reset();
				};

			deleteDialog = std::make_unique<DialogBox>("Cancellare il preset " + presetManager.getCurrentPreset() + " ? ", onAccept);

			addAndMakeVisible(*deleteDialog);
			deleteDialog->setBounds(0, 0, getWidth(), getHeight());

		}

		Service::PresetManager& presetManager;
		std::unique_ptr<DialogBox> deleteDialog;
		juce::TextButton saveButton, deleteButton, previousPresetButton, nextPresetButton, confirmButton, returnButton;
		juce::ComboBox presetList;
		std::unique_ptr<juce::FileChooser> fileChooser;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetPanel)
	};



} // namespace Gui