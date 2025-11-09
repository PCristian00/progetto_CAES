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
	gainSliderAttachment = std::make_unique<SliderAttachment>(apvts, "GAIN", gainSlider);

	setSliderParams(gainSlider, this);
}

GainComponent::~GainComponent()
{
}

void GainComponent::paint(juce::Graphics& g)
{
	/* This demo code just fills the component's background and
	   draws some placeholder text to get you started.

	   You should replace everything in this method with your own
	   drawing code..
	*/

	g.fillAll(juce::Colours::black);   // clear the background

	//g.fillAll(juce::Colours::darkred);
	//g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

	//g.setColour(juce::Colours::white);
	//g.setFont(juce::FontOptions(14.0f));
	//g.drawText("GainComponent", getLocalBounds(),
	//	juce::Justification::centred, true);   // draw some placeholder text
}

void GainComponent::resized()
{
	// This method is where you should set the bounds of any child
	// components that your component contains..

	// MODIFICARE IN FUTURO
	const auto bounds = getLocalBounds().reduced(10);
	const auto padding = 10;
	const int numSliders = 1;
	const auto sliderWidth = bounds.getWidth() / numSliders - padding;
	const auto sliderHeight = bounds.getHeight();
	const auto sliderXstart = padding;
	const auto sliderYstart = padding;

	setSliderBounds(gainSlider, sliderXstart, sliderYstart, sliderWidth, sliderHeight);

}
