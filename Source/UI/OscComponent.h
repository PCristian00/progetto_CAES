/*
  ==============================================================================

    OscComponent.h
    Created: 9 Nov 2025 5:04:34pm
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
class OscComponent  : public juce::Component
{
public:
    OscComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorId);
    ~OscComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    juce::ComboBox oscWaveSelector;
	std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscWaveSelectorAttachment;

	juce::Slider fmFreqSlider;
	juce::Slider fmDepthSlider;

	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> fmFreqSliderAttachment;

	juce::Label fmFreqLabel{ "FM Freq", "FM Freq"};

	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> fmDepthSliderAttachment;
	juce::Label fmDepthLabel{ "FM Depth", "FM Depth" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscComponent)
};
