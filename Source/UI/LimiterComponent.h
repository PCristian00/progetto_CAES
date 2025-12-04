/*
  ==============================================================================

    LimiterComponent.h
    Created: 4 Dec 2025 3:18:25pm
    Author:  crist

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Utils.h"

class LimiterComponent : public juce::Component
{
public:
    LimiterComponent(juce::AudioProcessorValueTreeState& apvts,
        juce::String thresholdId,
        juce::String releaseMsId);

    ~LimiterComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
	using LabeledSlider = utils::LabeledSlider;

	LabeledSlider threshLS;
	LabeledSlider releaseLS;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LimiterComponent)
};
