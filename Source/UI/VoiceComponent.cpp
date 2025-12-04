/*
  ==============================================================================

	VoiceComponent.cpp
	Created: 3 Dec 2025 2:38:22pm
	Author:  crist

  ==============================================================================
*/

#include "VoiceComponent.h"

VoiceComponent::VoiceComponent(juce::AudioProcessorValueTreeState& apvts, juce::String voiceID) :
	apvts(apvts), paramID(voiceID), numVoicesLS("", apvts, paramID, *this, juce::Slider::LinearBar, true)
{
}

VoiceComponent::~VoiceComponent() = default;

/**
 * Sfondo e bordo con titolo "Voices".
 */
void VoiceComponent::paint(juce::Graphics& g)
{
	g.fillAll(juce::Colours::black);
	utils::drawBorders(g, this, juce::Colours::lightpink, "Voices");
}
/**
 * Layout: lo slider occupa tutta l'area contenuti.
 */
void VoiceComponent::resized()
{
	const auto content = utils::getContentArea(this);
	numVoicesLS.setBounds(content);
}
