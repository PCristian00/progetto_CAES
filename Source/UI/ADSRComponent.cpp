/*
  ==============================================================================

	ADSRComponent.cpp
	Created: 8 Nov 2025 4:15:44pm
	Author:  crist

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ADSRComponent.h"
#include "Utils.h"

ADSRComponent::ADSRComponent(juce::String name,
	juce::AudioProcessorValueTreeState& apvts,
	juce::String attackId,
	juce::String decayId,
	juce::String sustainId,
	juce::String releaseId)
	: componentName(name)
	, attackLS("Attack", apvts, attackId, *this, juce::Slider::LinearBarVertical, true)
	, decayLS("Decay", apvts, decayId, *this, juce::Slider::LinearBarVertical, true)
	, sustainLS("Sustain", apvts, sustainId, *this, juce::Slider::LinearBarVertical, true)
	, releaseLS("Release", apvts, releaseId, *this, juce::Slider::LinearBarVertical, true)
{
}

/**
 * Sfondo e bordo con titolo del componente ADSR (il nome del componente passato al costruttore).
 */
void ADSRComponent::paint(juce::Graphics& g)
{
	g.fillAll(juce::Colours::black);
	utils::drawBorders(g, this, juce::Colours::deepskyblue, componentName);
}

/**
 * Layout: distribuisce uniformemente i 4 slider (Attack, Decay, Sustain, Release).
 */
void ADSRComponent::resized()
{
	const auto content = utils::getContentArea(this);

	utils::layoutVisibleRow(content.getX(), content.getY(),
		content.getWidth(), content.getHeight(),
		{ &attackLS, &decayLS, &sustainLS, &releaseLS });
}