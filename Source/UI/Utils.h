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

	// Altezza dell'area riservata al titolo/bordo
	inline int titleAreaHeight() noexcept { return 2 * padding; }

	// Impostazioni comuni per combo box + attachment
	// DA RIMUOVERE : ora usato il DropDown in FXComponent
	void setComboBoxParams(juce::ComboBox& comboBox, std::unique_ptr<ComboBoxAttachment>& attachment, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, juce::StringArray choices, juce::Component* parent = nullptr) noexcept;

	// Impostazioni comuni per button (create per coerenza con le altre, ma non usate)
	void setButton(juce::Button& button, const juce::String& buttonText, juce::Component* parent = nullptr) noexcept;

	// Bounds interni ridotti dal padding di contenitore
	juce::Rectangle<int> getBoundsWithPadding(juce::Component* parent = nullptr, int paddingOverride = 0) noexcept;
	// Area contenuti: bounds con padding, meno l'area del titolo/bordo, e con rientro interno
	juce::Rectangle<int> getContentArea(juce::Component* parent) noexcept;
	// Disegna bordo con titolo
	void drawBorders(juce::Graphics& g, juce::Component* parent, juce::Colour colour, juce::String title = "") noexcept;

	// Struttura riutilizzabile Slider + Label + Attachment
	struct LabeledSlider
	{
		juce::Slider slider;
		juce::Label  label;
		std::unique_ptr<SliderAttachment> attachment;

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

		void addTo(juce::Component& parent)
		{
			parent.addAndMakeVisible(label);
			parent.addAndMakeVisible(slider);
		}

		void attach(juce::AudioProcessorValueTreeState& state, const juce::String& paramID)
		{
			attachment = std::make_unique<SliderAttachment>(state, paramID, slider);
		}

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

		void setBounds(int x, int y, int width, int height, int labelHeight = padding * 2, int gap = static_cast<int> (padding / 2))
		{
			setBounds(juce::Rectangle<int>(x, y, width, height), labelHeight, gap);
		}

		void setVisible(bool v)
		{
			label.setVisible(v);
			slider.setVisible(v);
		}

		void setEnabled(bool e)
		{
			label.setEnabled(e);
			slider.setEnabled(e);
		}

		juce::Slider& getSlider() noexcept { return slider; }
		juce::Label& getLabel()  noexcept { return label; }
	};

	struct DropDown
	{
		juce::ComboBox cBox;
		std::unique_ptr<ComboBoxAttachment> attachment;
		juce::StringArray choices;

		explicit DropDown()
		{
			cBox.setJustificationType(juce::Justification::centred);
		}

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

		void addTo(juce::Component& parent)
		{
			parent.addAndMakeVisible(cBox);
		}

		void attach(juce::AudioProcessorValueTreeState& state, const juce::String& paramID)
		{
			attachment = std::make_unique<ComboBoxAttachment>(state, paramID, cBox);
		}

		// Configurazione rapida: scelte + LookAndFeel (opzionale) + giustificazione
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

		// Imposta l'elenco scelte (sostituisce quelle esistenti se richiesto)
		void setChoices(const juce::StringArray& items, bool clearExisting = true, int firstItemId = 1)
		{
			if (clearExisting)
				cBox.clear(juce::dontSendNotification);

			choices = items;
			if (!choices.isEmpty())
				cBox.addItemList(choices, firstItemId);
		}

		// Aggiunge una scelta singola (con id opzionale)
		void addChoice(const juce::String& item, int itemId = 0)
		{
			choices.add(item);
			const int newId = itemId > 0 ? itemId : (cBox.getNumItems() + 1);
			cBox.addItem(item, newId);
		}

		// Placeholder utili
		void setPlaceholderWhenNothingSelected(const juce::String& text)
		{
			cBox.setTextWhenNothingSelected(text);
		}

		void setPlaceholderWhenNoChoices(const juce::String& text)
		{
			cBox.setTextWhenNoChoicesAvailable(text);
		}

		// Layout: nessuna label, l'area va direttamente alla combo
		void setBounds(const juce::Rectangle<int>& area)
		{
			cBox.setBounds(area);
		}

		void setBounds(int x, int y, int width, int height)
		{
			setBounds(juce::Rectangle<int>(x, y, width, height));
		}

		void setVisible(bool v)
		{
			cBox.setVisible(v);
		}

		void setEnabled(bool e)
		{
			cBox.setEnabled(e);
		}

		juce::ComboBox& getComboBox() noexcept { return cBox; }
	};

	// Mostra una riga di sliders etichettati (LabeledSlider), distribuiti uniformemente
	void layoutVisibleRow(int x, int y, int totalWidth, int height, std::initializer_list<LabeledSlider*> sliders) noexcept;
	// Mostra una riga di sliders etichettati (LabeledSlider) preceduti da una combo box, distribuiti uniformemente
	void comboAndSliderRow(juce::ComboBox& comboBox, std::initializer_list<LabeledSlider*> sliders, juce::Component* parent, int x = Xstart, int y = Ystart, int totalWidth = 0, int height = 0) noexcept;
}