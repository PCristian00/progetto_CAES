/*
  ==============================================================================

	OscComponent.cpp
	Created: 9 Nov 2025 5:04:34pm
	Author:  crist

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OscComponent.h"

//==============================================================================
OscComponent::OscComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorId)
{
	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.

	juce::StringArray choices{ "Sine", "Saw", "Square" };
	oscWaveSelector.addItemList(choices, 1);
	addAndMakeVisible(oscWaveSelector);

	oscWaveSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, waveSelectorId, oscWaveSelector);
}

OscComponent::~OscComponent()
{
}

void OscComponent::paint(juce::Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background
}

void OscComponent::resized()
{
	oscWaveSelector.setBounds(10, 10, getWidth() - 20, 30);
}
