/*
  ==============================================================================

	PresetPanel.cpp
	Created: 23 Nov 2025 4:59:29pm
	Author:  crist

  ==============================================================================
*/

#include "PresetPanel.h"
#include "Utils.h"

namespace Gui
{
    /**
     * Pannello gestione preset: salva/cancella, naviga, e seleziona dalla lista.
     * Inizializza pulsanti e combo, carica la lista.
     */
    PresetPanel::PresetPanel(Service::PresetManager& pm) : presetManager(pm)
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

	PresetPanel::~PresetPanel()
	{
		saveButton.removeListener(this);
		deleteButton.removeListener(this);
		previousPresetButton.removeListener(this);
		nextPresetButton.removeListener(this);
		presetList.removeListener(this);
		if (dialogBox) dialogBox->close();
	}

    /**
     * Sfondo pannello.
     */
	void PresetPanel::paint(juce::Graphics& g)
	{
		g.fillAll(juce::Colours::black);
	}

    /**
     * Layout: pulsanti + combo, eventuale dialog overlay.
     */
	void PresetPanel::resized()
	{
		const auto container = getLocalBounds().reduced(4);
		auto bounds = container;

		saveButton.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.2f)).reduced(4));
		previousPresetButton.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.1f)).reduced(4));
		presetList.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.4f)).reduced(4));
		nextPresetButton.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.1f)).reduced(4));
		deleteButton.setBounds(bounds.reduced(4));

		if (dialogBox)
			dialogBox->setBounds(0, 0, getWidth(), getHeight());
	}

    /**
     * Popola la lista di preset (factory e utente), selezionando l'attuale.
     */
	void PresetPanel::loadPresetList()
	{
		presetList.clear(juce::dontSendNotification);

		const auto allPresets = presetManager.getAllPresets();
		const auto currentPreset = presetManager.getCurrentPreset();

		presetList.addSectionHeading("Preset di fabbrica");
		presetList.addItemList(presetManager.getEmbeddedPresets(), 1);
		presetList.addSeparator();
		presetList.addSectionHeading("Preset utente");
		presetList.addItemList(presetManager.getUserPresets(), presetList.getNumItems() + 1);

		presetList.setSelectedItemIndex(allPresets.indexOf(currentPreset), juce::dontSendNotification);
		checkPreset(presetManager.getCurrentPreset());
	}

    /**
     * Gestione click pulsanti: salva/cancella/naviga preset, poi aggiorna lista e UI.
     */
	void PresetPanel::buttonClicked(juce::Button* button)
	{
		if (button == &saveButton)
		{
			fileChooser = std::make_unique<juce::FileChooser>("Salva Preset",
				Service::PresetManager::defaultDirectory,
				"*." + Service::PresetManager::extension);

			auto saveFunction = [this](const juce::FileChooser& chooser)
				{
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

			if (presetManager.isEmbeddedPreset(current))
			{
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

    /**
     * Cambio selezione nella combo: carica preset selezionato e aggiorna UI.
     */
	void PresetPanel::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
	{
		if (comboBoxThatHasChanged == &presetList)
		{
			auto selectedPreset = presetList.getItemText(presetList.getSelectedItemIndex());
			presetManager.loadPreset(selectedPreset);
			checkPreset(selectedPreset);
		}
	}

    /**
     * Aggiorna abilitazione pulsante Delete e colori della lista
     * in base al tipo di preset (embedded/user) o assenza selezione.
     */
	void PresetPanel::checkPreset(juce::String preset)
	{
		if (presetManager.isEmbeddedPreset(preset))
		{
			deleteButton.setEnabled(false);
			presetList.setColour(presetList.textColourId, juce::Colours::greenyellow);
			presetList.setColour(presetList.backgroundColourId, juce::Colours::black);
		}
		else
			if (preset == "") {
				deleteButton.setEnabled(false);
				presetList.setColour(presetList.textColourId, defaultListTextColour);
				presetList.setColour(presetList.backgroundColourId, juce::Colours::black);
			}
			else
			{
				deleteButton.setEnabled(true);
				presetList.setColour(presetList.textColourId, defaultListTextColour);
				presetList.setColour(presetList.backgroundColourId, defaultListBgColour);
			}
	}

    /**
     * Mostra un dialog di conferma/avviso modale (overlay).
     */
	void PresetPanel::showDialogBox(juce::String msg, juce::String confirmText, juce::String returnText, std::function<void()> onAccept)
	{
		if (dialogBox)
			dialogBox->close();

		dialogBox = std::make_unique<DialogBox>(msg, confirmText, returnText, onAccept);
		addAndMakeVisible(*dialogBox);
		dialogBox->setBounds(0, 0, getWidth(), getHeight());
		dialogBox->show();
	}
} // namespace Gui