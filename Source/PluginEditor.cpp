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
	adsr(audioProcessor.apvts),
	gain(audioProcessor.apvts),
	osc(audioProcessor.apvts, "OSC"),
	filter(audioProcessor.apvts, "FILTER", "FILTERCUTOFF", "FILTERRES")
{
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.

	setSize(800, 600);

	addAndMakeVisible(adsr);
	addAndMakeVisible(gain);
	addAndMakeVisible(osc);
	addAndMakeVisible(filter);

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

	adsr.setBounds(0, 0, getWidth() / 3, getHeight());
	gain.setBounds(adsr.getRight(), 0, getWidth() / 3, getHeight());
	osc.setBounds(gain.getRight(), 0, getWidth() / 3, getHeight() / 2);
	filter.setBounds(osc.getX(), osc.getBottom(), getWidth() / 3, getHeight() / 2);

}
