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
	utils::comboAndSliderRow(filterTypeSelector, { &filterCutOffLS, &filterResonanceLS }, this);
}
