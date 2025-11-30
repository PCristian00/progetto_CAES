/*
  ==============================================================================

	GainComponent.cpp
	Created: 8 Nov 2025 6:56:15pm
	Author:  crist

  ==============================================================================
*/

#include <JuceHeader.h>
#include "GainComponent.h"
#include "Utils.h"

GainComponent::GainComponent(juce::AudioProcessorValueTreeState& apvtsIn, juce::String gainID)
	: apvts(apvtsIn)
	, paramID(gainID)
	, gainLS("", apvts, paramID, *this, juce::Slider::LinearBar, true)
{
}

GainComponent::~GainComponent() = default;

/**
 * Sfondo e bordo con titolo "Gain".
 */
void GainComponent::paint(juce::Graphics& g)
{
	g.fillAll(juce::Colours::black);
	utils::drawBorders(g, this, juce::Colours::orange, "Gain");
}

/**
 * Layout: lo slider occupa tutta l'area contenuti.
 */
void GainComponent::resized()
{
	const auto content = utils::getContentArea(this);
	gainLS.setBounds(content);
}
