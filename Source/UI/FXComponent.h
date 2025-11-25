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

	void showFXSliders(int x, int y, int width, int height, juce::Slider& a, juce::Slider& b, juce::Slider& c, juce::Slider& d);

	// void showFXSliders(juce::Component& a, juce::Component& b, juce::Component& c, juce::Component& d);

	juce::AudioProcessorValueTreeState& apvts;

	// Controls
	juce::ComboBox fxType;
	juce::ToggleButton bypass{ "Bypass" };
	juce::Slider wet, chRate, chDepth, chDelay, chFeedback, flRate, flDepth, flDelay, flFeedback, rvSize, rvDamp, rvWidth;

	juce::Label wetLabel{ {}, "Wet" };
	juce::Label chRateLabel{ {}, "Ch Rate" }, chDepthLabel{ {}, "Ch Depth" }, chDelayLabel{ {}, "Ch Delay" }, chFeedbackLabel{ {}, "Ch Feedback" };
	juce::Label flRateLabel{ {}, "Fl Rate" }, flDepthLabel{ {}, "Fl Depth" }, flDelayLabel{ {}, "Fl Delay" }, flFeedbackLabel{ {}, "Fl Feedback" };
	juce::Label rvSizeLabel{ {}, "Rev Size" }, rvDampLabel{ {}, "Rev Damp" }, rvWidthLabel{ {}, "Rev Width" };
	// Attachments
	using APVTS = juce::AudioProcessorValueTreeState;
	std::unique_ptr<APVTS::ComboBoxAttachment> fxTypeAttachment;
	std::unique_ptr<APVTS::ButtonAttachment>   bypassAttachment;
	std::unique_ptr<APVTS::SliderAttachment>   wetAttachment, chRateAttachment, chDepthAttachment, chDelayAttachment, chFeedbackAttachment;
	std::unique_ptr<APVTS::SliderAttachment>   flRateAttachment, flDepthAttachment, flDelayAttachment, flFeedbackAttachment;
	std::unique_ptr<APVTS::SliderAttachment>   rvSizeAttachment, rvDampAttachment, rvWidthAttachment;
};
