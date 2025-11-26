/*
  ==============================================================================

	FXComponent.h
	Created: 25 Nov 2025 12:49:57pm
	Author:  crist

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Utils.h"

class FXComponent : public juce::Component, private juce::ComboBox::Listener
{

	using LabeledSlider = utils::LabeledSlider;


public:
	FXComponent(juce::AudioProcessorValueTreeState& apvts);
	~FXComponent() override = default;

	void paint(juce::Graphics&) override;
	void resized() override;

private:
	void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;
	void updateVisibility();



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
