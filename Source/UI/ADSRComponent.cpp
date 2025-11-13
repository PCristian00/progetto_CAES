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
	setSliderParams(attackSlider, attackSliderAttachment, apvts, "ATTACK", attackLabel, this);
	setSliderParams(decaySlider, decaySliderAttachment, apvts, "DECAY", decayLabel, this);
	setSliderParams(sustainSlider, sustainSliderAttachment, apvts, "SUSTAIN", sustainLabel, this);
	setSliderParams(releaseSlider, releaseSliderAttachment, apvts, "RELEASE", releaseLabel, this);
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
	// const juce::Rectangle<int> bounds = getLocalBounds().reduced(padding);
	const int numSliders = 4;
	const int sliderWidth = getBoundsWithPadding(this).getWidth() / numSliders - padding;
	const int sliderHeight = getBoundsWithPadding(this).getHeight() - padding;
	const int sliderXstart = padding;
	const int sliderYstart = padding;

	setSliderBounds(attackSlider, attackLabel, sliderXstart, sliderYstart, sliderWidth, sliderHeight);
	setSliderBounds(decaySlider, decayLabel, attackSlider.getRight() + padding, sliderYstart, sliderWidth, sliderHeight);
	setSliderBounds(sustainSlider, sustainLabel, decaySlider.getRight() + padding, sliderYstart, sliderWidth, sliderHeight);
	setSliderBounds(releaseSlider, releaseLabel, sustainSlider.getRight() + padding, sliderYstart, sliderWidth, sliderHeight);
}