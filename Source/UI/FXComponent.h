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

	juce::AudioProcessorValueTreeState& apvts;

	juce::ComboBox fxType;
	juce::ToggleButton bypass{ "Bypass" };

	using APVTS = juce::AudioProcessorValueTreeState;
	std::unique_ptr<APVTS::ComboBoxAttachment> fxTypeAttachment;
	std::unique_ptr<APVTS::ButtonAttachment>   bypassAttachment;

	LabeledSlider wetLS;
	LabeledSlider chRateLS;
	LabeledSlider chDepthLS;
	LabeledSlider chDelayLS;
	LabeledSlider chFeedbackLS;
	LabeledSlider flRateLS;
	LabeledSlider flDepthLS;
	LabeledSlider flDelayLS;
	LabeledSlider flFeedbackLS;
	LabeledSlider rvSizeLS;
	LabeledSlider rvDampLS;
	LabeledSlider rvWidthLS;
};
