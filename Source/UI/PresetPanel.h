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
	class PresetPanel : public juce::Component,
		public juce::Button::Listener,
		public juce::ComboBox::Listener
	{
	public:
		PresetPanel(Service::PresetManager& pm) : presetManager(pm)
		{
			utils::setButton(saveButton, "Save", this);
			utils::setButton(deleteButton, "Delete", this);
			utils::setButton(previousPresetButton, "<", this);
			utils::setButton(nextPresetButton, ">", this);

			presetList.setTextWhenNothingSelected("Select Preset");
			presetList.setMouseCursor(juce::MouseCursor::PointingHandCursor);
			addAndMakeVisible(presetList);

			// Usato per checkPreset, viene memorizzato il colore del testo di default per reimpostarlo in seguito
			// Se serve, memorizzare i colori anche di sfondo e pulsanti
			defaultListTextColour = presetList.findColour(presetList.textColourId);
			defaultListBgColour = presetList.findColour(presetList.backgroundColourId);

			presetList.addListener(this);

			loadPresetList();
		}

		~PresetPanel()
		{
			saveButton.removeListener(this);
			deleteButton.removeListener(this);
			previousPresetButton.removeListener(this);
			nextPresetButton.removeListener(this);
			presetList.removeListener(this);
			if (dialogBox) dialogBox->close();
		}

		void paint(juce::Graphics& g) override {
			g.fillAll(juce::Colours::black);
		}

		void resized() override
		{
			const auto container = getLocalBounds().reduced(4);
			auto bounds = container;

			utils::setButtonBounds(saveButton, bounds.removeFromLeft(container.proportionOfWidth(0.2f)).reduced(4));
			utils::setButtonBounds(previousPresetButton, bounds.removeFromLeft(container.proportionOfWidth(0.1f)).reduced(4));
			utils::setComboBoxBounds(presetList, bounds.removeFromLeft(container.proportionOfWidth(0.4f)).reduced(4));
			utils::setButtonBounds(nextPresetButton, bounds.removeFromLeft(container.proportionOfWidth(0.1f)).reduced(4));
			utils::setButtonBounds(deleteButton, bounds.reduced(4));

			if (dialogBox)
				dialogBox->setBounds(0, 0, getWidth(), getHeight());
		}

	private:
		void loadPresetList()
		{
			presetList.clear(juce::dontSendNotification);

			const auto allPresets = presetManager.getAllPresets();
			const auto currentPreset = presetManager.getCurrentPreset();

			// Separatori tra preset di fabbrica e utente
			presetList.addSectionHeading("Preset di fabbrica");
			presetList.addItemList(presetManager.getEmbeddedPresets(), 1);
			presetList.addSeparator();
			presetList.addSectionHeading("Preset utente");
			presetList.addItemList(presetManager.getUserPresets(), presetList.getNumItems() + 1);

			// Versione senza separatori
			// presetList.addItemList(allPresets, 1);



			presetList.setSelectedItemIndex(allPresets.indexOf(currentPreset), juce::dontSendNotification);

			checkPreset(presetManager.getCurrentPreset());
		}

		void buttonClicked(juce::Button* button) override
		{
			if (button == &saveButton)
			{
				fileChooser = std::make_unique<juce::FileChooser>("Salva Preset", Service::PresetManager::defaultDirectory, "*." + Service::PresetManager::extension);

				auto saveFunction = [this](const juce::FileChooser& chooser) {
					auto file = chooser.getResult();
					if (file == juce::File{}) return;
					auto presetName = file.getFileNameWithoutExtension();

					if (!presetManager.isValidUserPresetName(presetName))
					{
						showDialogBox("Nome preset '" + presetName + "' non valido. Esiste un preset di fabbrica con questo nome.", "Ok", "", [this]() { dialogBox->close(); });
						return;
					}

					presetManager.savePreset(presetName);
					loadPresetList();
					};


				fileChooser->launchAsync(juce::FileBrowserComponent::saveMode, saveFunction);
				return;
			}
			else if (button == &deleteButton)
			{
				const auto current = presetManager.getCurrentPreset();
				if (current.isEmpty())
					return;

				// Nota: questo controllo non viene piu' raggiunto in quanto il pulsante viene disattiva da checkPreset().
				// Mantenuto qualora si volesse reimpostare il pulsante come sempre attivo in futuro.
				if (presetManager.isEmbeddedPreset(current)) {
					showDialogBox("Impossibile cancellare preset '" + current + "' (preset di fabbrica)", "Ok", "", [this]() { dialogBox->close(); });
					return;
				}

				juce::String msg = "Cancellare il preset '" + current + "' ?";

				auto deleteFunction = [this]()
					{
						presetManager.deletePreset(presetManager.getCurrentPreset());
						loadPresetList();

						if (dialogBox) dialogBox->close();
					};

				showDialogBox(msg, "Cancella preset", "Annulla", deleteFunction);
			}
			else if (button == &previousPresetButton)
			{
				presetList.setSelectedItemIndex(presetManager.loadPreviousPreset(), juce::dontSendNotification);
			}
			else if (button == &nextPresetButton)
			{
				presetList.setSelectedItemIndex(presetManager.loadNextPreset(), juce::dontSendNotification);
			}

			checkPreset(presetManager.getCurrentPreset());
		}

		void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override
		{
			if (comboBoxThatHasChanged == &presetList)
			{
				auto selectedPreset = presetList.getItemText(presetList.getSelectedItemIndex());
				presetManager.loadPreset(selectedPreset);
				checkPreset(selectedPreset);
			}
		}


		void checkPreset(juce::String preset) {
			if (presetManager.isEmbeddedPreset(preset)) {
				// QUI SI POSSONO AGGIUNGERE ALTRE PERSONALIZZAZIONI PER INDICARE SE IL PRESET E' DI FABBRICA
				deleteButton.setEnabled(false);
				presetList.setColour(presetList.textColourId, juce::Colours::greenyellow);
				presetList.setColour(presetList.backgroundColourId, juce::Colours::darkblue);
			}
			else {
				deleteButton.setEnabled(true);
				presetList.setColour(presetList.textColourId, defaultListTextColour);
				presetList.setColour(presetList.backgroundColourId, defaultListBgColour);
			}
		}

		void showDialogBox(juce::String msg, juce::String confirmText, juce::String returnText, std::function<void()> onAccept)
		{
			if (dialogBox)
			{
				dialogBox->close();
			}

			dialogBox = std::make_unique<DialogBox>(msg, confirmText, returnText, onAccept);
			addAndMakeVisible(*dialogBox);

			dialogBox->setBounds(0, 0, getWidth(), getHeight());
			dialogBox->show();
		}

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