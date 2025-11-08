/*
  ==============================================================================

    ADSRComponent.h
    Created: 8 Nov 2025 4:15:44pm
    Author:  crist

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ADSRComponent  : public juce::Component
{
public:
    ADSRComponent(juce::AudioProcessorValueTreeState& apvts);
    ~ADSRComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    juce::Slider attackSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider releaseSlider;

    void setSliderParams(juce::Slider& slider);
    void setSliderBounds(juce::Slider& slider, int x, int y, int width, int height);

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    std::unique_ptr<SliderAttachment> attackSliderAttachment;
    std::unique_ptr<SliderAttachment> decaySliderAttachment;
    std::unique_ptr<SliderAttachment> sustainSliderAttachment;
    std::unique_ptr<SliderAttachment> releaseSliderAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ADSRComponent)
};
