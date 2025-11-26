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
	const int Xstart = static_cast<int>(padding * 1.5f);
	const int Ystart = padding * 4;

	// Struttura riutilizzabile Slider + Label + Attachment
	struct LabeledSlider
	{
		juce::Slider slider;
		juce::Label  label;
		std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;

		// Costruttore: imposta il testo della label e qualche default sensato
		explicit LabeledSlider(const juce::String& labelText = {})
		{
			label.setText(labelText, juce::dontSendNotification);
			label.setJustificationType(juce::Justification::centred);
			label.setColour(juce::Label::textColourId, juce::Colours::white);

			slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 18);
		}

		// Aggiunge i controlli al parent
		void addTo(juce::Component& parent)
		{
			parent.addAndMakeVisible(label);
			parent.addAndMakeVisible(slider);
		}

		// Collega il parametro APVTS
		void attach(juce::AudioProcessorValueTreeState& state, const juce::String& paramID)
		{
			attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(state, paramID, slider);
		}

		// Configura stile e opzionale LookAndFeel
		void configure(juce::Slider::SliderStyle style,
			bool showTextBox = true,
			juce::LookAndFeel* laf = nullptr)
		{
			slider.setSliderStyle(style);
			if (showTextBox)
				slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 18);
			else
				slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

			if (laf != nullptr)
				slider.setLookAndFeel(laf);
		}

		// Layout: label sopra, slider sotto
		void setBounds(const juce::Rectangle<int>& area, int labelHeight = 16, int gap = 4)
		{
			auto r = area;
			label.setBounds(r.removeFromTop(labelHeight));
			r.removeFromTop(gap);
			slider.setBounds(r);
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

	void setSliderParams(juce::Slider& slider, std::unique_ptr<SliderAttachment>& attachment, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, juce::Label& label, juce::Component* parent = nullptr, SliderStyle style = SliderStyle::LinearBarVertical) noexcept;

	void setSliderParams(juce::Slider& slider, std::unique_ptr<SliderAttachment>& attachment, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, juce::Label* label = nullptr, juce::Component* parent = nullptr, SliderStyle style = SliderStyle::LinearBarVertical) noexcept;

	void setSliderBounds(juce::Slider& slider, int x, int y, int width, int height, juce::Label& label) noexcept;
	void setSliderBounds(juce::Slider& slider, int x, int y, int width, int height, juce::Label* label = nullptr) noexcept;

	void setComboBoxParams(juce::ComboBox& comboBox, std::unique_ptr<ComboBoxAttachment>& attachment, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, juce::StringArray choices, juce::Component* parent = nullptr) noexcept;

	void setComboBoxBounds(juce::ComboBox& comboBox, juce::Rectangle<int> size) noexcept;
	void setComboBoxBounds(juce::ComboBox& comboBox, int x, int y, int width, int height) noexcept;

	void setButton(juce::Button& button, const juce::String& buttonText, juce::Component* parent = nullptr) noexcept;
	void setButtonBounds(juce::Button& button, juce::Rectangle<int> size) noexcept;

	juce::Rectangle<int> getBoundsWithPadding(juce::Component* parent = nullptr, int paddingOverride = 0) noexcept;

	void drawBorders(juce::Graphics& g, juce::Component* parent, juce::Colour colour, juce::String title = "") noexcept;
}
