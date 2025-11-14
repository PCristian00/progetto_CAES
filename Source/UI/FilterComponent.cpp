/*
  ==============================================================================

	FilterComponent.cpp
	Created: 14 Nov 2025 4:32:15pm
	Author:  crist

  ==============================================================================
*/

#include "FilterComponent.h"

//==============================================================================
FilterComponent::FilterComponent(juce::AudioProcessorValueTreeState& apvts, juce::String filterTypeSelectorId)
{
	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.
	juce::StringArray choices{ "Low-Pass", "High-Pass", "Band-Pass" };
	setComboBoxParams(filterTypeSelector, filterTypeSelectorAttachment, apvts, filterTypeSelectorId, choices, this);

	setSliderParams(filterCutOffSlider, filterCutOffSliderAttachment, apvts, "FILTERCUTOFF", filterFreqLabel, this);
	setSliderParams(filterResonanceSlider, filterResonanceSliderAttachment, apvts, "FILTERRES", filterResonanceLabel, this);
}

FilterComponent::~FilterComponent()
{
}

void FilterComponent::paint(juce::Graphics& g)
{
	g.fillAll(juce::Colours::black);   // clear the background
}

void FilterComponent::resized()
{
	// This method is where you should set the bounds of any child
	// components that your component contains..
	const int numSliders = 2;
	const int sliderWidth = getBoundsWithPadding(this).getWidth() / numSliders - padding;
	const int sliderHeight = getBoundsWithPadding(this).getHeight() / numSliders - padding;
	const int sliderXstart = padding;
	const int sliderYstart = padding;

	setComboBoxBounds(filterTypeSelector, padding, padding, sliderWidth * numSliders + padding, (2 * padding));
	setSliderBounds(filterCutOffSlider, filterFreqLabel, sliderXstart, filterTypeSelector.getBottom() + padding, sliderWidth, sliderHeight);
	setSliderBounds(filterResonanceSlider, filterResonanceLabel, filterCutOffSlider.getRight() + padding, filterTypeSelector.getBottom() + padding, sliderWidth, sliderHeight);

}
