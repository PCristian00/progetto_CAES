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
/**
 * Pannello Filtro: selezione tipo e controlli cutoff/risonanza.
 * Collega i controlli agli APVTS params.
 */
FilterComponent::FilterComponent(juce::AudioProcessorValueTreeState& apvts, juce::String filterTypeSelectorId, juce::String filterCutOffId, juce::String filterResonanceId)
	: filterCutOffLS("Filter Cutoff", apvts, filterCutOffId, *this, juce::Slider::LinearBarVertical, true)
	, filterResonanceLS("Filter Resonance", apvts, filterResonanceId, *this, juce::Slider::LinearBarVertical, true)
	, filterTypeSelector(apvts, filterTypeSelectorId, *this, { "Low-Pass","Band-Pass","High-Pass" })
{
}

FilterComponent::~FilterComponent() {}

void FilterComponent::paint(juce::Graphics& g)
{
	g.fillAll(juce::Colours::black);
	utils::drawBorders(g, this, juce::Colours::darkorchid, "Filter");
}

/**
 * Layout: combo tipo + riga slider (cutoff, resonance).
 */
void FilterComponent::resized()
{
	utils::comboAndSliderRow(filterTypeSelector, { &filterCutOffLS, &filterResonanceLS }, this);
}
