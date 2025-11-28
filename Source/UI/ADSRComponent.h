/*
  ==============================================================================

	ADSRComponent.h
	Created: 8 Nov 2025 4:15:44pm
	Author:  crist

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Utils.h"

//==============================================================================
/*
*/
class ADSRComponent : public juce::Component
{
public:
	ADSRComponent(juce::String name, juce::AudioProcessorValueTreeState& apvts, juce::String attackId, juce::String decayId, juce::String sustainId, juce::String releaseId);
	~ADSRComponent() override = default;

	void paint(juce::Graphics&) override;
	void resized() override;

private:
	using LabeledSlider = utils::LabeledSlider;

	juce::String componentName;

	LabeledSlider attackLS;
	LabeledSlider decayLS;
	LabeledSlider sustainLS;
	LabeledSlider releaseLS;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADSRComponent)
};
