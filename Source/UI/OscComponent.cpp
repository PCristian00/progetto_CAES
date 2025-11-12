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
	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.

	juce::StringArray choices{ "Sine", "Saw", "Square" };
	oscWaveSelector.addItemList(choices, 1);
	addAndMakeVisible(oscWaveSelector);

	oscWaveSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, waveSelectorId, oscWaveSelector);

	fmFreqSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "FMFREQ", fmFreqSlider);

	setSliderParams(fmFreqSlider, this);

	// AGGIUNGERE A UTILS
	fmFreqLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
	fmFreqLabel.setFont(15.0f);
	fmFreqLabel.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(fmFreqLabel);
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
	oscWaveSelector.setBounds(10, 10, getWidth() - 20, 30);
	// fmFreqSlider.setBounds(0, 80, 100, 90);
	setSliderBounds(fmFreqSlider, 10, 80, 100, 90);
	fmFreqLabel.setBounds(fmFreqSlider.getX(), fmFreqSlider.getY() - 20, fmFreqSlider.getWidth(), 20);
}
