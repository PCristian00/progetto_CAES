/*
  ==============================================================================

	ADSRComponent.cpp
	Created: 8 Nov 2025 4:15:44pm
	Author:  crist

  ==============================================================================
*/


#include "ADSRComponent.h"


//==============================================================================
ADSRComponent::ADSRComponent(juce::AudioProcessorValueTreeState& apvts)
{
	attackSliderAttachment = std::make_unique<SliderAttachment>(apvts, "ATTACK", attackSlider);
	decaySliderAttachment = std::make_unique<SliderAttachment>(apvts, "DECAY", decaySlider);
	sustainSliderAttachment = std::make_unique<SliderAttachment>(apvts, "SUSTAIN", sustainSlider);
	releaseSliderAttachment = std::make_unique<SliderAttachment>(apvts, "RELEASE", releaseSlider);

	setSliderParams(attackSlider, this);
	setSliderParams(decaySlider, this);
	setSliderParams(sustainSlider, this);
	setSliderParams(releaseSlider, this);
}

ADSRComponent::~ADSRComponent()
{
}

void ADSRComponent::paint(juce::Graphics& g)
{
	g.fillAll(juce::Colours::black);
}

void ADSRComponent::resized()
{
	// This method is where you should set the bounds of any child
	// components that your component contains..
	const auto bounds = getLocalBounds().reduced(10);
	const auto padding = 10;
	const int numSliders = 4;
	const auto sliderWidth = bounds.getWidth() / numSliders - padding;
	const auto sliderHeight = bounds.getHeight();
	const auto sliderXstart = padding;
	const auto sliderYstart = padding;

	setSliderBounds(attackSlider, sliderXstart, sliderYstart, sliderWidth, sliderHeight);
	setSliderBounds(decaySlider, attackSlider.getRight() + padding, sliderYstart, sliderWidth, sliderHeight);
	setSliderBounds(sustainSlider, decaySlider.getRight() + padding, sliderYstart, sliderWidth, sliderHeight);
	setSliderBounds(releaseSlider, sustainSlider.getRight() + padding, sliderYstart, sliderWidth, sliderHeight);
}
