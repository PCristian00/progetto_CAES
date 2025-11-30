/*
  ==============================================================================

	GainComponent.h
	Created: 8 Nov 2025 6:56:15pm
	Author:  crist

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Utils.h"

class GainComponent : public juce::Component
{
public:
	GainComponent(juce::AudioProcessorValueTreeState& apvts, juce::String gainID);
	~GainComponent() override;

	void paint(juce::Graphics&) override;
	void resized() override;

private:
	using LabeledSlider = utils::LabeledSlider;
	juce::AudioProcessorValueTreeState& apvts;
	juce::String paramID;

	LabeledSlider gainLS;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainComponent)
};
