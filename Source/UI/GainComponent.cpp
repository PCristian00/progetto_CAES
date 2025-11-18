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
#include "../Parameters.h"

//==============================================================================
GainComponent::GainComponent(juce::AudioProcessorValueTreeState& apvts, juce::String gainID)
{
	utils::setSliderParams(gainSlider, gainSliderAttachment, apvts, gainID, gainLabel, this, SliderStyle::LinearBar);
}

GainComponent::~GainComponent() {}

void GainComponent::paint(juce::Graphics& g)
{
	g.fillAll(juce::Colours::black);
	utils::drawBorders(g, this, juce::Colours::darkolivegreen, "Gain");
}

void GainComponent::resized()
{
	const int numSliders = 1;
	/*const int sliderXstart = utils::padding + 5;
	const int sliderYstart = utils::padding * 4;*/

	const int sliderWidth = utils::getBoundsWithPadding(this).getWidth() / numSliders - utils::padding;
	const int sliderHeight = utils::getBoundsWithPadding(this).getHeight() - utils::Ystart;


	utils::setSliderBounds(gainSlider, gainLabel, utils::Xstart, utils::Ystart, sliderWidth, sliderHeight);
}
