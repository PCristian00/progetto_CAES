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
	oscWaveSelector.addItemList(choices, 1);
	addAndMakeVisible(oscWaveSelector);

	oscWaveSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, waveSelectorId, oscWaveSelector);

	setSliderParams(fmFreqSlider, fmFreqSliderAttachment, apvts, "FMFREQ", fmFreqLabel, this);
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
	oscWaveSelector.setBounds(padding, padding, getWidth() - (2 * padding), (3 * padding));
	setSliderBounds(fmFreqSlider, fmFreqLabel, padding, 80, 100, 90);
}
