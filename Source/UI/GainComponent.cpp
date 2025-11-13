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
	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.
	// gainSliderAttachment = std::make_unique<SliderAttachment>(apvts, "GAIN", gainSlider);

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
	// This method is where you should set the bounds of any child
	// components that your component contains..

	// MODIFICARE IN FUTURO: forse pezzi inutili (quali numSliders...)
	const auto bounds = getLocalBounds().reduced(10);
	const auto padding = 10;
	const int numSliders = 1;
	const auto sliderWidth = bounds.getWidth() / numSliders - padding;
	const auto sliderHeight = bounds.getHeight();
	const auto sliderXstart = padding;
	const auto sliderYstart = padding;

	setSliderBounds(gainSlider, gainLabel, sliderXstart, sliderYstart, sliderWidth, sliderHeight);

}
