/*
  ==============================================================================

	OscComponent.cpp
	Created: 9 Nov 2025 5:04:34pm
	Author:  crist

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OscComponent.h"
#include "Utils.h"
#include "../Parameters.h"

//==============================================================================
/**
 * Pannello Oscillatore: selezione forma d'onda e controlli FM (freq, depth).
 * Collega slider e combo agli parametri APVTS.
 */
OscComponent::OscComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorId)
    : fmFreqLS("FM Freq", apvts, parameters::FM_FREQUENCY, *this, juce::Slider::LinearBarVertical, true)
    , fmDepthLS("FM Depth", apvts, parameters::FM_DEPTH, *this, juce::Slider::LinearBarVertical, true)
	, oscWaveSelector(apvts, waveSelectorId, *this, { "Sine","Saw","Square" })
{
    fmFreqLS.setUnitSuffix(" Hz");

	// I restanti slider sono lineari e non hanno unita'
}

OscComponent::~OscComponent() {}

void OscComponent::paint(juce::Graphics& g)
{
	g.fillAll(juce::Colours::black);
	utils::drawBorders(g, this, juce::Colours::darkgoldenrod, "Oscillator");
}

/**
 * Layout: combo onda + riga di slider FM.
 */
void OscComponent::resized()
{
	utils::comboAndSliderRow(oscWaveSelector, { &fmFreqLS, &fmDepthLS }, this);
}