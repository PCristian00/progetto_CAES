/*
  ==============================================================================

	OscComponent.cpp
	Created: 9 Nov 2025 5:04:34pm
	Author:  crist

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OscComponent.h"
#include "Utils.h"
#include "../Parameters.h"

//==============================================================================
OscComponent::OscComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorId)
{
	juce::StringArray choices{ "Sine","Saw","Square" };
	utils::setComboBoxParams(oscWaveSelector, oscWaveSelectorAttachment, apvts, waveSelectorId, choices, this);

	utils::setSliderParams(fmFreqSlider, fmFreqSliderAttachment, apvts, parameters::FM_FREQUENCY_PARAM_ID, fmFreqLabel, this);
	utils::setSliderParams(fmDepthSlider, fmDepthSliderAttachment, apvts, parameters::FM_DEPTH_PARAM_ID, fmDepthLabel, this);
}

OscComponent::~OscComponent() {}

void OscComponent::paint(juce::Graphics& g)
{
	g.fillAll(juce::Colours::black);
	utils::drawBorders(g, this, juce::Colours::darkgoldenrod, "Oscillator");
}

void OscComponent::resized()
{
	const int numSliders = 2;

	//const int sliderXstart = utils::padding + 5;
	//const int sliderYstart = utils::padding * 4;

	const int comboBoxWidth = utils::getBoundsWithPadding(this).getWidth() - utils::padding;
	const int comboBoxHeight = utils::getBoundsWithPadding(this).getHeight() / 6 - utils::padding;

	const int sliderWidth = (comboBoxWidth / numSliders) - (utils::padding / numSliders);
	const int sliderHeight = utils::getBoundsWithPadding(this).getHeight() - (utils::Ystart + comboBoxHeight + utils::padding);

	utils::setComboBoxBounds(oscWaveSelector, utils::Xstart, utils::Ystart, comboBoxWidth, comboBoxHeight);
	utils::setSliderBounds(fmFreqSlider,  utils::Xstart, oscWaveSelector.getBottom() + utils::padding, sliderWidth, sliderHeight, fmFreqLabel);
	utils::setSliderBounds(fmDepthSlider,  fmFreqSlider.getRight() + utils::padding, fmFreqSlider.getY(), sliderWidth, sliderHeight, fmDepthLabel);
}
