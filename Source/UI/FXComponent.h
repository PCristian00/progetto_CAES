/*
  ==============================================================================

	FXComponent.h
	Created: 25 Nov 2025 12:49:57pm
	Author:  crist

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class FXComponent : public juce::Component, private juce::ComboBox::Listener
{
public:
	FXComponent(juce::AudioProcessorValueTreeState& apvts);
	~FXComponent() override = default;

	void paint(juce::Graphics&) override;
	void resized() override;

private:
	void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;
	void updateVisibility();

	// Struttura riutilizzabile Slider + Label + Attachment (spostare in utils!!!)
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

	void layoutVisibleRow(int x, int y, int totalWidth, int height, std::initializer_list<LabeledSlider*> sliders);

	juce::AudioProcessorValueTreeState& apvts;

	// Controls
	juce::ComboBox fxType;
	juce::ToggleButton bypass{ "Bypass" };

	using APVTS = juce::AudioProcessorValueTreeState;
	std::unique_ptr<APVTS::ComboBoxAttachment> fxTypeAttachment;
	std::unique_ptr<APVTS::ButtonAttachment>   bypassAttachment;

	// LabeledSlider (sostituiscono vecchi slider + label + attachment)
	LabeledSlider wetLS{ "Wet" };

	LabeledSlider chRateLS{ "Ch Rate" };
	LabeledSlider chDepthLS{ "Ch Depth" };
	LabeledSlider chDelayLS{ "Ch Delay" };
	LabeledSlider chFeedbackLS{ "Ch Feedback" };

	LabeledSlider flRateLS{ "Fl Rate" };
	LabeledSlider flDepthLS{ "Fl Depth" };
	LabeledSlider flDelayLS{ "Fl Delay" };
	LabeledSlider flFeedbackLS{ "Fl Feedback" };

	LabeledSlider rvSizeLS{ "Rev Size" };
	LabeledSlider rvDampLS{ "Rev Damp" };
	LabeledSlider rvWidthLS{ "Rev Width" };
};
