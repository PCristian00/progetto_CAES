/*
  ==============================================================================

	ADSRComponent.cpp
	Created: 8 Nov 2025 4:15:44pm
	Author:  crist

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ADSRComponent.h"
#include "Utils.h"

//==============================================================================
ADSRComponent::ADSRComponent(juce::String name, juce::AudioProcessorValueTreeState& apvts, juce::String attackId, juce::String decayId, juce::String sustainId, juce::String releaseId)
{
	componentName = name;

	utils::setSliderParams(attackSlider, attackSliderAttachment, apvts, attackId, attackLabel, this);
	utils::setSliderParams(decaySlider, decaySliderAttachment, apvts, decayId, decayLabel, this);
	utils::setSliderParams(sustainSlider, sustainSliderAttachment, apvts, sustainId, sustainLabel, this);
	utils::setSliderParams(releaseSlider, releaseSliderAttachment, apvts, releaseId, releaseLabel, this);
}

ADSRComponent::~ADSRComponent() {}

void ADSRComponent::paint(juce::Graphics& g)
{
	auto bounds = utils::getBoundsWithPadding(this);
	auto labelSpace = bounds.removeFromTop(2 * utils::padding);

	g.fillAll(juce::Colours::black);
	g.setColour(juce::Colours::darkcyan);
	g.setFont(15.0f);
	g.drawText(componentName, labelSpace.withX(utils::padding), juce::Justification::left);
	g.drawRoundedRectangle(bounds.toFloat(), 5.0f, 2.0f);
}

void ADSRComponent::resized()
{
	const int numSliders = 4;
	const int sliderWidth = utils::getBoundsWithPadding(this).getWidth() / numSliders - utils::padding;
	const int sliderHeight = utils::getBoundsWithPadding(this).getHeight() - (numSliders * utils::padding);
	const int sliderXstart = utils::padding + 5;
	const int sliderYstart = utils::padding * numSliders;

	utils::setSliderBounds(attackSlider, attackLabel, sliderXstart, sliderYstart, sliderWidth, sliderHeight);
	utils::setSliderBounds(decaySlider, decayLabel, attackSlider.getRight() + utils::padding, sliderYstart, sliderWidth, sliderHeight);
	utils::setSliderBounds(sustainSlider, sustainLabel, decaySlider.getRight() + utils::padding, sliderYstart, sliderWidth, sliderHeight);
	utils::setSliderBounds(releaseSlider, releaseLabel, sustainSlider.getRight() + utils::padding, sliderYstart, sliderWidth, sliderHeight);
}