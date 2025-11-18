/*
  ==============================================================================

	FilterComponent.cpp
	Created: 14 Nov 2025 4:32:15pm
	Author:  crist

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterComponent.h"
#include "Utils.h"

//==============================================================================
FilterComponent::FilterComponent(juce::AudioProcessorValueTreeState& apvts, juce::String filterTypeSelectorId, juce::String filterCutOffId, juce::String filterResonanceId)
{
	juce::StringArray choices{ "Low-Pass","Band-Pass","High-Pass" };
	utils::setComboBoxParams(filterTypeSelector, filterTypeSelectorAttachment, apvts, filterTypeSelectorId, choices, this);

	utils::setSliderParams(filterCutOffSlider, filterCutOffSliderAttachment, apvts, filterCutOffId, filterFreqLabel, this);
	utils::setSliderParams(filterResonanceSlider, filterResonanceSliderAttachment, apvts, filterResonanceId, filterResonanceLabel, this);
}

FilterComponent::~FilterComponent() {}

void FilterComponent::paint(juce::Graphics& g)
{
	g.fillAll(juce::Colours::black);
}

void FilterComponent::resized()
{
	const int numSliders = 2;
	const int sliderWidth = utils::getBoundsWithPadding(this).getWidth() / numSliders - utils::padding;
	const int sliderHeight = utils::getBoundsWithPadding(this).getHeight() / numSliders - utils::padding;
	const int sliderXstart = utils::padding;

	utils::setComboBoxBounds(filterTypeSelector, utils::padding, utils::padding, sliderWidth * numSliders + utils::padding, (2 * utils::padding));
	utils::setSliderBounds(filterCutOffSlider, filterFreqLabel, sliderXstart, filterTypeSelector.getBottom() + utils::padding, sliderWidth, sliderHeight);
	utils::setSliderBounds(filterResonanceSlider, filterResonanceLabel, filterCutOffSlider.getRight() + utils::padding, filterTypeSelector.getBottom() + utils::padding, sliderWidth, sliderHeight);
}
