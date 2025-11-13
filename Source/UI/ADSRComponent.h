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
using namespace utils;

//==============================================================================
/*
*/
class ADSRComponent : public juce::Component
{
public:
	ADSRComponent(juce::AudioProcessorValueTreeState& apvts);
	~ADSRComponent() override;

	void paint(juce::Graphics&) override;
	void resized() override;

private:

	juce::Slider attackSlider;
	juce::Slider decaySlider;
	juce::Slider sustainSlider;
	juce::Slider releaseSlider;

	using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

	std::unique_ptr<SliderAttachment> attackSliderAttachment;
	std::unique_ptr<SliderAttachment> decaySliderAttachment;
	std::unique_ptr<SliderAttachment> sustainSliderAttachment;
	std::unique_ptr<SliderAttachment> releaseSliderAttachment;

	juce::Label attackLabel{ "Attack", "Attack" };
	juce::Label decayLabel{ "Decay", "Decay" };
	juce::Label sustainLabel{ "Sustain", "Sustain" };
	juce::Label releaseLabel{ "Release", "Release" };

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADSRComponent)
};
