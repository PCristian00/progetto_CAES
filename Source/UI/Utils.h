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

	// Altezza dell'area riservata al titolo/bordo (coerente con drawBorders)
	inline int titleAreaHeight() noexcept { return 2 * padding; }

	// Struttura riutilizzabile Slider + Label + Attachment
	struct LabeledSlider
	{
		juce::Slider slider;
		juce::Label  label;
		std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;

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
			attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(state, paramID, slider);
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

	void setSliderParams(juce::Slider& slider, std::unique_ptr<SliderAttachment>& attachment, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, juce::Label& label, juce::Component* parent = nullptr, SliderStyle style = SliderStyle::LinearBarVertical) noexcept;
	void setSliderParams(juce::Slider& slider, std::unique_ptr<SliderAttachment>& attachment, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, juce::Label* label = nullptr, juce::Component* parent = nullptr, SliderStyle style = SliderStyle::LinearBarVertical) noexcept;

	void setSliderBounds(juce::Slider& slider, int x, int y, int width, int height, juce::Label& label) noexcept;
	void setSliderBounds(juce::Slider& slider, int x, int y, int width, int height, juce::Label* label = nullptr) noexcept;

	void setComboBoxParams(juce::ComboBox& comboBox, std::unique_ptr<ComboBoxAttachment>& attachment, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, juce::StringArray choices, juce::Component* parent = nullptr) noexcept;

	void setComboBoxBounds(juce::ComboBox& comboBox, juce::Rectangle<int> size) noexcept;
	void setComboBoxBounds(juce::ComboBox& comboBox, int x, int y, int width, int height) noexcept;

	void setButton(juce::Button& button, const juce::String& buttonText, juce::Component* parent = nullptr) noexcept;
	void setButtonBounds(juce::Button& button, juce::Rectangle<int> size) noexcept;

	// Bounds interni ridotti dal padding di contenitore
	juce::Rectangle<int> getBoundsWithPadding(juce::Component* parent = nullptr, int paddingOverride = 0) noexcept;

	// Area contenuti: bounds con padding, meno l'area del titolo/bordo (uniforma lo spazio di testata)
	juce::Rectangle<int> getContentArea(juce::Component* parent) noexcept;

	void drawBorders(juce::Graphics& g, juce::Component* parent, juce::Colour colour, juce::String title = "") noexcept;

	void layoutVisibleRow(int x, int y, int totalWidth, int height, std::initializer_list<LabeledSlider*> sliders) noexcept;

	void comboAndSliderRow(juce::ComboBox& comboBox, std::initializer_list<LabeledSlider*> sliders, juce::Component* parent, int x = Xstart, int y = Ystart, int totalWidth = 0, int height = 0) noexcept;
}
