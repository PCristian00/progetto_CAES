/*
  ==============================================================================

	FilterComponent.h
	Created: 14 Nov 2025 4:32:15pm
	Author:  crist

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Utils.h"

//==============================================================================
///
/*
*/
class FilterComponent : public juce::Component
{
public:
	FilterComponent(juce::AudioProcessorValueTreeState& apvts, juce::String filterTyperSelectorId, juce::String filterCutOffId, juce::String filterResonanceId);
	~FilterComponent() override;

	void paint(juce::Graphics&) override;
	void resized() override;

private:
	using LabeledSlider = utils::LabeledSlider;

	utils::DropDown filterTypeSelector;

	LabeledSlider filterCutOffLS;
	LabeledSlider filterResonanceLS;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterComponent)
};
