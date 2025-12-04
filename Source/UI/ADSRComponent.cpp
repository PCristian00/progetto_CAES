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
	attackLS.setUnitSuffix(" s");
	decayLS.setUnitSuffix(" s");
	releaseLS.setUnitSuffix(" s");

	// I restanti slider sono lineari e non hanno unita'

	// Theme colour based on border colour
	const auto borderColour = (componentName == "Amp Envelope") ? utils::oscCol : utils::filtCol;
	attackLS.setThemeColour(borderColour);
	decayLS.setThemeColour(borderColour);
	sustainLS.setThemeColour(borderColour);
	releaseLS.setThemeColour(borderColour);
}

/**
 * Sfondo e bordo con titolo del componente ADSR (il nome del componente passato al costruttore).
 */
void ADSRComponent::paint(juce::Graphics& g)
{
	g.fillAll(juce::Colours::black);

	if (componentName == "Amp Envelope")
		utils::drawBorders(g, this, utils::oscCol, componentName);
	else
		utils::drawBorders(g, this, utils::filtCol, componentName);
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