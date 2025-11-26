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

//==============================================================================
/*
*/
class OscComponent : public juce::Component
{
public:
	OscComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorId);
	~OscComponent() override;

	void paint(juce::Graphics&) override;
	void resized() override;

private:
	using LabeledSlider = utils::LabeledSlider;

	juce::ComboBox oscWaveSelector;
	std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscWaveSelectorAttachment;

	// Sostituisce juce::Slider + Attachment + Label
	LabeledSlider fmFreqLS;
	LabeledSlider fmDepthLS;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscComponent)
};
