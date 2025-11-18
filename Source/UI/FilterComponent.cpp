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
	utils::drawBorders(g, this, juce::Colours::darkorchid, "Filter");

}

void FilterComponent::resized()
{
	const int numSliders = 2;

	const int sliderXstart = utils::padding + 5;
	const int sliderYstart = utils::padding * 4;

	const int comboBoxWidth = utils::getBoundsWithPadding(this).getWidth() - utils::padding;
	const int comboBoxHeight = utils::getBoundsWithPadding(this).getHeight() / 6 - utils::padding;

	const int sliderWidth = (comboBoxWidth / numSliders) - (utils::padding / numSliders);
	const int sliderHeight = utils::getBoundsWithPadding(this).getHeight() - (sliderYstart + comboBoxHeight + utils::padding);


	utils::setComboBoxBounds(filterTypeSelector, sliderXstart, sliderYstart, comboBoxWidth, comboBoxHeight);
	utils::setSliderBounds(filterCutOffSlider, filterFreqLabel, sliderXstart, filterTypeSelector.getBottom() + utils::padding, sliderWidth, sliderHeight);
	utils::setSliderBounds(filterResonanceSlider, filterResonanceLabel, filterCutOffSlider.getRight() + utils::padding, filterCutOffSlider.getY(), sliderWidth, sliderHeight);
}
