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
	setSliderParams(gainSlider, gainSliderAttachment, apvts, "GAIN", gainLabel, this);
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
	const auto bounds = getLocalBounds().reduced(padding);
	const int numSliders = 1;
	const auto sliderWidth = bounds.getWidth() / numSliders - padding;
	const auto sliderHeight = bounds.getHeight();
	const auto sliderXstart = padding;
	const auto sliderYstart = padding;

	setSliderBounds(gainSlider, gainLabel, sliderXstart, sliderYstart, sliderWidth, sliderHeight);

}
