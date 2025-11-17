/*
  ==============================================================================

	GainComponent.cpp
	Created: 8 Nov 2025 6:56:15pm
	Author:  crist

  ==============================================================================
*/

#include <JuceHeader.h>
#include "GainComponent.h"

//==============================================================================
GainComponent::GainComponent(juce::AudioProcessorValueTreeState& apvts)
{
	setSliderParams(gainSlider, gainSliderAttachment, apvts, GAIN_PARAM_ID, gainLabel, this);
}

GainComponent::~GainComponent()
{
}

void GainComponent::paint(juce::Graphics& g)
{
	g.fillAll(juce::Colours::black);
}

void GainComponent::resized()
{
	const int numSliders = 1;
	const int sliderWidth = getBoundsWithPadding(this).getWidth() / numSliders - padding;
	const int sliderHeight = getBoundsWithPadding(this).getHeight() - padding;
	const int sliderXstart = padding;
	const int sliderYstart = padding;

	setSliderBounds(gainSlider, gainLabel, sliderXstart, sliderYstart, sliderWidth, sliderHeight);
}
