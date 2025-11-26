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
	: filterCutOffLS("Filter Cutoff", apvts, filterCutOffId, *this, juce::Slider::LinearBarVertical, true)
	, filterResonanceLS("Filter Resonance", apvts, filterResonanceId, *this, juce::Slider::LinearBarVertical, true)
{
	juce::StringArray choices{ "Low-Pass","Band-Pass","High-Pass" };
	utils::setComboBoxParams(filterTypeSelector, filterTypeSelectorAttachment, apvts, filterTypeSelectorId, choices, this);
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

	const int comboBoxWidth = utils::getBoundsWithPadding(this).getWidth() - utils::padding;
	const int comboBoxHeight = utils::getBoundsWithPadding(this).getHeight() / 6 - utils::padding;

	const int sliderWidth = (comboBoxWidth / numSliders) - (utils::padding / numSliders);
	const int sliderHeight = utils::getBoundsWithPadding(this).getHeight() - (utils::Ystart + comboBoxHeight + utils::padding);

	utils::setComboBoxBounds(filterTypeSelector, utils::Xstart, utils::Ystart, comboBoxWidth, comboBoxHeight);

	filterCutOffLS.setBounds(utils::Xstart, filterTypeSelector.getBottom() + utils::padding, sliderWidth, sliderHeight);
	filterResonanceLS.setBounds(utils::Xstart + sliderWidth + utils::padding, filterTypeSelector.getBottom() + utils::padding, sliderWidth, sliderHeight);
}
