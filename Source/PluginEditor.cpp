/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SubSynthAudioProcessorEditor::SubSynthAudioProcessorEditor(SubSynthAudioProcessor& p)
	: AudioProcessorEditor(&p),
	audioProcessor(p),
	adsr("Amp Envelope", audioProcessor.apvts, ATTACK_PARAM_ID, DECAY_PARAM_ID, SUSTAIN_PARAM_ID, RELEASE_PARAM_ID),
	gain(audioProcessor.apvts),
	osc(audioProcessor.apvts, OSCILLATOR_PARAM_ID),
	filter(audioProcessor.apvts, FILTER_TYPE_PARAM_ID, FILTER_CUTOFF_PARAM_ID, FILTER_RESONANCE_PARAM_ID)
{
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.

	setSize(800, 600);

	addAndMakeVisible(adsr);
	addAndMakeVisible(gain);
	addAndMakeVisible(osc);
	addAndMakeVisible(filter);
	addAndMakeVisible(modAdsr);

}

SubSynthAudioProcessorEditor::~SubSynthAudioProcessorEditor()
{
}

//==============================================================================
void SubSynthAudioProcessorEditor::paint(juce::Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void SubSynthAudioProcessorEditor::resized()
{

	adsr.setBounds(0, 0, getWidth() / 3, getHeight()/2);
	modAdsr.setBounds(0, adsr.getBottom(), getWidth() / 3, getHeight() / 2);
	gain.setBounds(adsr.getRight(), 0, getWidth() / 3, getHeight());
	osc.setBounds(gain.getRight(), 0, getWidth() / 3, getHeight() / 2);
	filter.setBounds(osc.getX(), osc.getBottom(), getWidth() / 3, getHeight() / 2);

}
