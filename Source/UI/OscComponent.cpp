/*
  ==============================================================================

	OscComponent.cpp
	Created: 9 Nov 2025 5:04:34pm
	Author:  crist

  ==============================================================================
*/

#include "OscComponent.h"

//==============================================================================

// In caso di più oscillatori, si deve passare un id diverso per ognuno dei fmFreq e fmDepth
OscComponent::OscComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorId)
{
	juce::StringArray choices{ "Sine", "Saw", "Square" };
	setComboBoxParams(oscWaveSelector, oscWaveSelectorAttachment, apvts, waveSelectorId, choices, this);

	setSliderParams(fmFreqSlider, fmFreqSliderAttachment, apvts, "FMFREQ", fmFreqLabel, this);
	setSliderParams(fmDepthSlider, fmDepthSliderAttachment, apvts, "FMDEPTH", fmDepthLabel, this);
}

OscComponent::~OscComponent()
{
}

void OscComponent::paint(juce::Graphics& g)
{
	g.fillAll(juce::Colours::black);   // clear the background

	//g.setColour(juce::Colours::white);
	//g.drawRect(getLocalBounds(), 1);   // draw an outline around the component
}

void OscComponent::resized()
{
	const int numSliders = 2;
	const int sliderWidth = getBoundsWithPadding(this).getWidth() / numSliders - padding;
	const int sliderHeight = getBoundsWithPadding(this).getHeight() / numSliders - padding;
	const int sliderXstart = padding;
	const int sliderYstart = padding;

	setComboBoxBounds(oscWaveSelector, padding, padding, getWidth() - (2 * padding), (2 * padding));
	setSliderBounds(fmFreqSlider, fmFreqLabel, sliderXstart, oscWaveSelector.getBottom() + padding, sliderWidth, sliderHeight);
	setSliderBounds(fmDepthSlider, fmDepthLabel, fmFreqSlider.getRight() + padding, oscWaveSelector.getBottom() + padding, sliderWidth, sliderHeight);
}
