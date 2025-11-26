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

void GainComponent::paint(juce::Graphics& g)
{
	g.fillAll(juce::Colours::black);
	utils::drawBorders(g, this, juce::Colours::orange, "Gain");
}

void GainComponent::resized()
{
	// Area contenuti uniforme (niente moltiplicazioni manuali di padding)
	const auto content = utils::getContentArea(this);
	gainLS.setBounds(content);
}
