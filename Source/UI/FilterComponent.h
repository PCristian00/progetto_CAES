/*
  ==============================================================================

    FilterComponent.h
    Created: 14 Nov 2025 4:32:15pm
    Author:  crist

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
///
/*
*/
class FilterComponent  : public juce::Component
{
public:
    FilterComponent(juce::AudioProcessorValueTreeState& apvts, juce::String filterTyperSelectorId, juce::String filterCutOffId, juce::String filterResonanceId);
    ~FilterComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    juce::ComboBox filterTypeSelector{"Filter Type"};
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterTypeSelectorAttachment;

    juce::Slider filterCutOffSlider;
    juce::Slider filterResonanceSlider;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterCutOffSliderAttachment;
    juce::Label filterFreqLabel{ "Filter Cutoff", "Filter Cutoff" };

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterResonanceSliderAttachment;
    juce::Label filterResonanceLabel{ "Filter Resonance", "Filter Resonance" };


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterComponent)
};
