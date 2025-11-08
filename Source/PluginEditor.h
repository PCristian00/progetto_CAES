/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/ADSRComponent.h"

//==============================================================================
/**
*/
class SubSynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SubSynthAudioProcessorEditor (SubSynthAudioProcessor&);
    ~SubSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

	void setSliderParams(juce::Slider& slider);

	void setSliderBounds(juce::Slider& slider, int x, int y, int width, int height);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.

	SubSynthAudioProcessor& audioProcessor;

	ADSRComponent adsr;

	juce::Slider gainSlider;
	
	juce::ComboBox oscSelector;

   

	using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

	std::unique_ptr<SliderAttachment> gainSliderAttachment;

	

	std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscSelectorAttachment;


    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SubSynthAudioProcessorEditor)
};
