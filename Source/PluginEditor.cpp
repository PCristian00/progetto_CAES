/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SubSynthAudioProcessorEditor::SubSynthAudioProcessorEditor(SubSynthAudioProcessor& p)
	: AudioProcessorEditor(&p), audioProcessor(p), adsr(audioProcessor.apvts), gain(audioProcessor.apvts)
{
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.

	setSize(800, 600);
		
	oscSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "OSC", oscSelector);

	addAndMakeVisible(adsr);
	addAndMakeVisible(gain);

}

SubSynthAudioProcessorEditor::~SubSynthAudioProcessorEditor()
{
}

//==============================================================================
void SubSynthAudioProcessorEditor::paint(juce::Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

	/*g.setColour(juce::Colours::white);
	g.setFont(juce::FontOptions(15.0f));*/
	// g.drawFittedText("Sub Synth", getLocalBounds(), juce::Justification::centred, 1);
}

void SubSynthAudioProcessorEditor::resized()
{

	adsr.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight());
	gain.setBounds(0, 0, getWidth() / 2, getHeight());

	// Temporaneo: creare file .cpp e .h anche per gain

}
