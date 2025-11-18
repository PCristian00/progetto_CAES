/*
  ==============================================================================

	OscComponent.cpp
	Created: 9 Nov 2025 5:04:34pm
	Author:  crist

  ==============================================================================
*/

#include "OscComponent.h"
#include "Utils.h"
#include "../Parameters.h"

//==============================================================================
OscComponent::OscComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorId)
{
	juce::StringArray choices{"Sine","Saw","Square"};
	utils::setComboBoxParams(oscWaveSelector, oscWaveSelectorAttachment, apvts, waveSelectorId, choices, this);

	utils::setSliderParams(fmFreqSlider, fmFreqSliderAttachment, apvts, parameters::FM_FREQUENCY_PARAM_ID, fmFreqLabel, this);
	utils::setSliderParams(fmDepthSlider, fmDepthSliderAttachment, apvts, parameters::FM_DEPTH_PARAM_ID, fmDepthLabel, this);
}

OscComponent::~OscComponent() {}

void OscComponent::paint(juce::Graphics& g)
{
	g.fillAll(juce::Colours::black);
}

void OscComponent::resized()
{
	const int numSliders =2;
	const int sliderWidth = utils::getBoundsWithPadding(this).getWidth() / numSliders - utils::padding;
	const int sliderHeight = utils::getBoundsWithPadding(this).getHeight() / numSliders - utils::padding;
	const int sliderXstart = utils::padding;
	// const int sliderYstart = utils::padding; // non usato direttamente

	utils::setComboBoxBounds(oscWaveSelector, utils::padding, utils::padding, sliderWidth * numSliders + utils::padding, (2 * utils::padding));
	utils::setSliderBounds(fmFreqSlider, fmFreqLabel, sliderXstart, oscWaveSelector.getBottom() + utils::padding, sliderWidth, sliderHeight);
	utils::setSliderBounds(fmDepthSlider, fmDepthLabel, fmFreqSlider.getRight() + utils::padding, oscWaveSelector.getBottom() + utils::padding, sliderWidth, sliderHeight);
}
