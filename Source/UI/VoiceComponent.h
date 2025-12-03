/*
  ==============================================================================

	VoiceComponent.h
	Created: 3 Dec 2025 2:38:22pm
	Author:  crist

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Utils.h"

class VoiceComponent : public juce::Component
{
public:
	VoiceComponent(juce::AudioProcessorValueTreeState& apvts, juce::String voiceID);
	~VoiceComponent() override;

	void paint(juce::Graphics&) override;
	void resized() override;

private:
	using LabeledSlider = utils::LabeledSlider;
	juce::AudioProcessorValueTreeState& apvts;
	juce::String paramID;

	LabeledSlider numVoicesLS;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VoiceComponent)
};
