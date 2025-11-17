/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/ADSRComponent.h"
#include "UI/GainComponent.h"
#include "UI/OscComponent.h"
#include "UI/FilterComponent.h"

//==============================================================================
/**
*/
class SubSynthAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
	SubSynthAudioProcessorEditor(SubSynthAudioProcessor&);
	~SubSynthAudioProcessorEditor() override;

	//==============================================================================
	void paint(juce::Graphics&) override;
	void resized() override;

private:
	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.

	SubSynthAudioProcessor& audioProcessor;

	ADSRComponent adsr;
	GainComponent gain;
	OscComponent osc;
	FilterComponent filter;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SubSynthAudioProcessorEditor)
};
