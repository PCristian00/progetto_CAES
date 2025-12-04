/*
  ==============================================================================

	Utils.h
	Created: 8 Nov 2025 5:39:14pm
	Author:  crist

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
using SliderStyle = juce::Slider::SliderStyle;

namespace utils
{
	const int padding = 10;
	// Margine interno tra contenuti e bordo disegnato (separato dal padding generale)
	const int contentPadding = 12;

	const int Xstart = static_cast<int>(padding * 1.5f);
	const int Ystart = padding * 4;

	// Colore dei componenti di oscillatore e inviluppo
	const juce::Colour oscCol = juce::Colours::mediumvioletred;
	// Colore dei componenti dei filtri
	const juce::Colour filtCol = juce::Colours::cornflowerblue;
	// Colore di FX Component
	const juce::Colour fxCol = juce::Colours::indianred;
	// Colore dei restanti componenti
	const juce::Colour miscCol = juce::Colours::blueviolet;
	// Colore specifico per PresetPanel
	const juce::Colour presetCol = juce::Colours::slateblue;



	// Altezza dell'area riservata al titolo/bordo
	inline int titleAreaHeight() noexcept { return 2 * padding; }

	// Impostazioni comuni per button
	void setButton(juce::Button& button, const juce::String& buttonText, juce::Component* parent = nullptr) noexcept;

	// Bounds interni ridotti dal padding di contenitore
	juce::Rectangle<int> getBoundsWithPadding(juce::Component* parent = nullptr, int paddingOverride = 0) noexcept;
	// Area contenuti: bounds con padding, meno l'area del titolo/bordo, e con rientro interno
	juce::Rectangle<int> getContentArea(juce::Component* parent) noexcept;
	// Disegna bordo con titolo
	void drawBorders(juce::Graphics& g, juce::Component* parent, juce::Colour colour, juce::String title = "") noexcept;

	// Struttura riutilizzabile Slider + Label + Attachment
	/**
	 * LabeledSlider: incapsula uno Slider con la sua Label e l'attachment APVTS.
	 * Fornisce costruttori rapidi per: sola label, label+attachment, aggiunta al parent,
	 * configurazione stile/LookAndFeel. Espone metodi per layout e visibilita'.
	 */
	struct LabeledSlider
	{
		juce::Slider slider;
		juce::Label  label;
		std::unique_ptr<SliderAttachment> attachment;

		/** Costruttore base: crea label (se testo non vuoto) e slider con TextBoxBelow. */
		explicit LabeledSlider(const juce::String& labelText = {})
		{
			label.setText(labelText, juce::dontSendNotification);
			label.setJustificationType(juce::Justification::centred);
			label.setColour(juce::Label::textColourId, juce::Colours::white);

			slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 5 * padding, static_cast<int> (2.5f * padding));
		}

		LabeledSlider(const juce::String& labelText, juce::AudioProcessorValueTreeState& state, const juce::String& paramID)
			: LabeledSlider(labelText)
		{
			attach(state, paramID);
		}

		LabeledSlider(const juce::String& labelText, juce::AudioProcessorValueTreeState& state, const juce::String& paramID, juce::Component& parent)
			: LabeledSlider(labelText, state, paramID) {
			addTo(parent);
		}

		LabeledSlider(const juce::String& labelText, juce::AudioProcessorValueTreeState& state, const juce::String& paramID, juce::Component& parent, juce::Slider::SliderStyle style, bool showTextBox, juce::LookAndFeel* laf = nullptr)
			: LabeledSlider(labelText, state, paramID, parent)
		{
			configure(style, showTextBox, laf);
		}

		/** Aggiunge label e slider al parent. */
		void addTo(juce::Component& parent)
		{
			parent.addAndMakeVisible(label);
			parent.addAndMakeVisible(slider);
		}

		/** Collega lo slider al parametro APVTS tramite SliderAttachment. */
		void attach(juce::AudioProcessorValueTreeState& state, const juce::String& paramID)
		{
			attachment = std::make_unique<SliderAttachment>(state, paramID, slider);
		}

		/** Configura stile slider, visibilita' del text box e LookAndFeel opzionale. */
		void configure(juce::Slider::SliderStyle style,
			bool showTextBox = true,
			juce::LookAndFeel* laf = nullptr)
		{
			slider.setSliderStyle(style);
			if (showTextBox)
				slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 5 * padding, static_cast<int> (2.5f * padding));
			else
				slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

			if (laf != nullptr)
				slider.setLookAndFeel(laf);
		}

		void setBounds(const juce::Rectangle<int>& area, int labelHeight = padding * 2, int gap = static_cast<int> (padding / 2))
		{
			auto r = area;
			if (label.getText() != "") {
				label.setBounds(r.removeFromTop(labelHeight));
				r.removeFromBottom(gap);
			}
			slider.setBounds(r);
		}

		/** Overload di setBounds con coordinate intere. */
		void setBounds(int x, int y, int width, int height, int labelHeight = padding * 2, int gap = static_cast<int> (padding / 2))
		{
			setBounds(juce::Rectangle<int>(x, y, width, height), labelHeight, gap);
		}

		/** Aggiunge un suffisso al valore dello slider. Utile per unita' di misura. */
		void setUnitSuffix(juce::String unit) {
			this->slider.setTextValueSuffix(unit);
		}

		/** Imposta visibilita' coerente di label e slider. */
		void setVisible(bool v)
		{
			label.setVisible(v);
			slider.setVisible(v);
		}

		/** Abilita/disabilita label e slider. */
		void setEnabled(bool e)
		{
			label.setEnabled(e);
			slider.setEnabled(e);
		}

		/** Accesso diretto allo slider. */
		juce::Slider& getSlider() noexcept { return slider; }
		/** Accesso diretto alla label. */
		juce::Label& getLabel()  noexcept { return label; }

		/** Applica una tema colore allo slider/label basato sul colore del bordo. */
		void setThemeColour(juce::Colour base)
		{
			// Slider palette
			slider.setColour(juce::Slider::thumbColourId, base);
			slider.setColour(juce::Slider::trackColourId, base.darker(0.3f));
			slider.setColour(juce::Slider::rotarySliderFillColourId, base);
			slider.setColour(juce::Slider::rotarySliderOutlineColourId, base.darker(0.5f));
			slider.setColour(juce::Slider::backgroundColourId, base.withAlpha(0.15f));
			slider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
			slider.setColour(juce::Slider::textBoxBackgroundColourId, base.withAlpha(0.25f));
			slider.setColour(juce::Slider::textBoxOutlineColourId, base.darker(0.6f));

			// Label text colour harmonised
			label.setColour(juce::Label::textColourId, base.brighter(0.6f));
		}
	};

	/**
	 * DropDown: wrapper per ComboBox con attachment APVTS e gestione scelte.
	 * Fornisce configurazione rapida (items + LookAndFeel), placeholder,
	 * aggiunta dinamica di elementi e layout semplice.
	 */
	struct DropDown
	{
		juce::ComboBox cBox;
		std::unique_ptr<ComboBoxAttachment> attachment;
		juce::StringArray choices;
		// LookAndFeel dedicato per permettere colori personalizzati del PopupMenu
		std::unique_ptr<juce::LookAndFeel_V4> ownedLaf;

		~DropDown()
		{
			if (ownedLaf && &cBox.getLookAndFeel() == ownedLaf.get())
				cBox.setLookAndFeel(nullptr);
		}

		/** Costruttore base: inizializza giustificazione centrata. */
		explicit DropDown()
		{
			cBox.setJustificationType(juce::Justification::centred);
		}

		/** Costruttore con attachment immediato al parametro APVTS. */
		DropDown(juce::AudioProcessorValueTreeState& state, const juce::String& paramID)
			: DropDown()
		{
			attach(state, paramID);
		}

		DropDown(juce::AudioProcessorValueTreeState& state, const juce::String& paramID, juce::Component& parent)
			: DropDown(state, paramID)
		{
			addTo(parent);
		}

		DropDown(juce::AudioProcessorValueTreeState& state,
			const juce::String& paramID,
			juce::Component& parent,
			const juce::StringArray& items,
			juce::LookAndFeel* laf = nullptr,
			int firstItemId = 1)
			: DropDown(state, paramID, parent)
		{
			configure(items, laf, firstItemId);
		}

		/** Aggiunge la combo al parent e la rende visibile. */
		void addTo(juce::Component& parent)
		{
			parent.addAndMakeVisible(cBox);
		}

		/** Collega la ComboBox al parametro APVTS. */
		void attach(juce::AudioProcessorValueTreeState& state, const juce::String& paramID)
		{
			attachment = std::make_unique<ComboBoxAttachment>(state, paramID, cBox);
		}

		/** Configurazione rapida: scelte + LookAndFeel (opzionale) + giustificazione. */
		void configure(const juce::StringArray& items = {},
			juce::LookAndFeel* laf = nullptr,
			int firstItemId = 1,
			juce::Justification justification = juce::Justification::centred)
		{
			if (!items.isEmpty())
				setChoices(items, true, firstItemId);

			cBox.setJustificationType(justification);

			if (laf != nullptr)
				cBox.setLookAndFeel(laf);
		}

		/** Imposta l'elenco scelte (sostituisce quelle esistenti se richiesto). */
		void setChoices(const juce::StringArray& items, bool clearExisting = true, int firstItemId = 1)
		{
			if (clearExisting)
				cBox.clear(juce::dontSendNotification);

			choices = items;
			if (!choices.isEmpty())
				cBox.addItemList(choices, firstItemId);

			if (cBox.getSelectedId() == 0 && cBox.getNumItems() > 0)
				cBox.setSelectedId(firstItemId, juce::dontSendNotification);
		}

		/** Aggiunge una singola scelta con ID opzionale (autocalcolato se 0). */
		void addChoice(const juce::String& item, int itemId = 0)
		{
			choices.add(item);
			const int newId = itemId > 0 ? itemId : (cBox.getNumItems() + 1);
			cBox.addItem(item, newId);

			if (cBox.getSelectedId() == 0)
				cBox.setSelectedId(newId, juce::dontSendNotification);
		}

		/** Testo placeholder quando nulla e' selezionato. */
		void setPlaceholderWhenNothingSelected(const juce::String& text)
		{
			cBox.setTextWhenNothingSelected(text);
		}

		/** Testo placeholder quando non ci sono scelte disponibili. */
		void setPlaceholderWhenNoChoices(const juce::String& text)
		{
			cBox.setTextWhenNoChoicesAvailable(text);
		}

		/** Layout: nessuna label, l'area va direttamente alla combo. */
		void setBounds(const juce::Rectangle<int>& area)
		{
			cBox.setBounds(area);
		}

		/** Overload setBounds con coordinate. */
		void setBounds(int x, int y, int width, int height)
		{
			setBounds(juce::Rectangle<int>(x, y, width, height));
		}

		/** Imposta visibilita' combo. */
		void setVisible(bool v)
		{
			cBox.setVisible(v);
		}

		/** Abilita/disabilita combo. */
		void setEnabled(bool e)
		{
			cBox.setEnabled(e);
		}

		/** Accesso diretto alla ComboBox. */
		juce::ComboBox& getComboBox() noexcept { return cBox; }

		/** Applica una tema colore alla combo basato sul colore del bordo. */
		void setThemeColour(juce::Colour base)
		{
			cBox.setColour(juce::ComboBox::backgroundColourId, base.withAlpha(0.35f));
			cBox.setColour(juce::ComboBox::textColourId, juce::Colours::white);
			cBox.setColour(juce::ComboBox::outlineColourId, base.darker(0.5f));
			cBox.setColour(juce::ComboBox::buttonColourId, base);
			cBox.setColour(juce::ComboBox::arrowColourId, base.brighter(0.6f));

			// Popup menu (tendina) colours: usa un LookAndFeel dedicato per non impattare globalmente
			if (!ownedLaf)
			{
				ownedLaf = std::make_unique<juce::LookAndFeel_V4>();
				cBox.setLookAndFeel(ownedLaf.get());
			}

			ownedLaf->setColour(juce::PopupMenu::backgroundColourId, base.withAlpha(0.85f));
			ownedLaf->setColour(juce::PopupMenu::textColourId, juce::Colours::white);
			ownedLaf->setColour(juce::PopupMenu::highlightedBackgroundColourId, base.brighter(0.3f));
			ownedLaf->setColour(juce::PopupMenu::highlightedTextColourId, juce::Colours::black);
			ownedLaf->setColour(juce::PopupMenu::headerTextColourId, base.brighter(0.8f));
		}
	};

	// Mostra una riga di sliders etichettati (LabeledSlider), distribuiti uniformemente
	void layoutVisibleRow(int x, int y, int totalWidth, int height, std::initializer_list<LabeledSlider*> sliders) noexcept;
	// Mostra una riga di sliders etichettati (LabeledSlider) preceduti da una combo box, distribuiti uniformemente
	void comboAndSliderRow(utils::DropDown& dd, std::initializer_list<LabeledSlider*> sliders, juce::Component* parent, int x = 0, int y = 0, int totalWidth = 0, int height = 0) noexcept;
}