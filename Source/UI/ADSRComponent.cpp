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

ADSRComponent::ADSRComponent(juce::String name,
	juce::AudioProcessorValueTreeState& apvts,
	juce::String attackId,
	juce::String decayId,
	juce::String sustainId,
	juce::String releaseId)
	: componentName(name)
	, attackLS("Attack", apvts, attackId, *this, juce::Slider::LinearBarVertical, true)
	, decayLS("Decay", apvts, decayId, *this, juce::Slider::LinearBarVertical, true)
	, sustainLS("Sustain", apvts, sustainId, *this, juce::Slider::LinearBarVertical, true)
	, releaseLS("Release", apvts, releaseId, *this, juce::Slider::LinearBarVertical, true)
{
}

void ADSRComponent::paint(juce::Graphics& g)
{
	g.fillAll(juce::Colours::black);
	utils::drawBorders(g, this, juce::Colours::white, componentName);
}

void ADSRComponent::resized()
{
	const int numSliders = 4;
	const auto padded = utils::getBoundsWithPadding(this);
	const int sliderWidth = padded.getWidth() / numSliders - utils::padding;
	const int sliderHeight = padded.getHeight() - (utils::Ystart - utils::padding);

	int x = utils::Xstart;
	const int y = utils::Ystart;

	attackLS.setBounds(x, y, sliderWidth, sliderHeight);
	x += sliderWidth + utils::padding;
	decayLS.setBounds(x, y, sliderWidth, sliderHeight);
	x += sliderWidth + utils::padding;
	sustainLS.setBounds(x, y, sliderWidth, sliderHeight);
	x += sliderWidth + utils::padding;
	releaseLS.setBounds(x, y, sliderWidth, sliderHeight);
}