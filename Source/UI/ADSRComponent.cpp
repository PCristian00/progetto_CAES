/*
  ==============================================================================

	ADSRComponent.cpp
	Created: 8 Nov 2025 4:15:44pm
	Author:  crist

  ==============================================================================
*/


#include "ADSRComponent.h"


//==============================================================================
ADSRComponent::ADSRComponent(juce::String name, juce::AudioProcessorValueTreeState& apvts, juce::String attackId, juce::String decayId, juce::String sustainId, juce::String releaseId)
{
	componentName = name;

	setSliderParams(attackSlider, attackSliderAttachment, apvts, attackId, attackLabel, this);
	setSliderParams(decaySlider, decaySliderAttachment, apvts, decayId, decayLabel, this);
	setSliderParams(sustainSlider, sustainSliderAttachment, apvts, sustainId, sustainLabel, this);
	setSliderParams(releaseSlider, releaseSliderAttachment, apvts, releaseId, releaseLabel, this);
}

ADSRComponent::~ADSRComponent()
{
}

void ADSRComponent::paint(juce::Graphics& g)
{

	auto bounds = getBoundsWithPadding(this);

	auto labelSpace = bounds.removeFromTop(2 * padding);

	g.fillAll(juce::Colours::black);
	g.setColour(juce::Colours::white);
	g.setFont(15.0f);
	g.drawText(componentName, labelSpace.withX(padding), juce::Justification::left);
	g.drawRoundedRectangle(bounds.toFloat(), 5.0f, 2.0f);
}

void ADSRComponent::resized()
{
	const int numSliders = 4;
	const int sliderWidth = getBoundsWithPadding(this).getWidth() / numSliders - padding;
	const int sliderHeight = getBoundsWithPadding(this).getHeight() - (numSliders * padding);
	const int sliderXstart = padding + 5;
	const int sliderYstart = padding * numSliders;

	setSliderBounds(attackSlider, attackLabel, sliderXstart, sliderYstart, sliderWidth, sliderHeight);
	setSliderBounds(decaySlider, decayLabel, attackSlider.getRight() + padding, sliderYstart, sliderWidth, sliderHeight);
	setSliderBounds(sustainSlider, sustainLabel, decaySlider.getRight() + padding, sliderYstart, sliderWidth, sliderHeight);
	setSliderBounds(releaseSlider, releaseLabel, sustainSlider.getRight() + padding, sliderYstart, sliderWidth, sliderHeight);
}