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
OscComponent::OscComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorId)
	: fmFreqLS("FM Freq", apvts, parameters::FM_FREQUENCY_PARAM_ID, *this, juce::Slider::LinearBarVertical, true)
	, fmDepthLS("FM Depth", apvts, parameters::FM_DEPTH_PARAM_ID, *this, juce::Slider::LinearBarVertical, true)
{
	juce::StringArray choices{ "Sine","Saw","Square" };
	utils::setComboBoxParams(oscWaveSelector, oscWaveSelectorAttachment, apvts, waveSelectorId, choices, this);
}

OscComponent::~OscComponent() {}

void OscComponent::paint(juce::Graphics& g)
{
	g.fillAll(juce::Colours::black);
	utils::drawBorders(g, this, juce::Colours::darkgoldenrod, "Oscillator");
}

void OscComponent::resized()
{
	utils::comboAndSliderRow(oscWaveSelector, { &fmFreqLS, &fmDepthLS }, this);
}