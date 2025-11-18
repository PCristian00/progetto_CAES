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


	g.fillAll(juce::Colours::black);
	utils::drawBorders(g, this, juce::Colours::white, componentName);

}

void ADSRComponent::resized()
{
	const int numSliders = 4;

	/*const int sliderXstart = utils::padding + 5;
	const int sliderYstart = utils::padding * 4;*/

	const int sliderWidth = utils::getBoundsWithPadding(this).getWidth() / numSliders - utils::padding;
	const int sliderHeight = utils::getBoundsWithPadding(this).getHeight() - (utils::Ystart);


	utils::setSliderBounds(attackSlider, attackLabel, utils::Xstart, utils::Ystart, sliderWidth, sliderHeight);
	utils::setSliderBounds(decaySlider, decayLabel, attackSlider.getRight() + utils::padding, utils::Ystart, sliderWidth, sliderHeight);
	utils::setSliderBounds(sustainSlider, sustainLabel, decaySlider.getRight() + utils::padding, utils::Ystart, sliderWidth, sliderHeight);
	utils::setSliderBounds(releaseSlider, releaseLabel, sustainSlider.getRight() + utils::padding, utils::Ystart, sliderWidth, sliderHeight);
}