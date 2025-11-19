/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Parameters.h"

//==============================================================================
SubSynthAudioProcessorEditor::SubSynthAudioProcessorEditor(SubSynthAudioProcessor& p)
	: AudioProcessorEditor(&p),
	audioProcessor(p),
	adsr("Amp Envelope", audioProcessor.apvts, parameters::ATTACK_PARAM_ID, parameters::DECAY_PARAM_ID, parameters::SUSTAIN_PARAM_ID, parameters::RELEASE_PARAM_ID),
	gain(audioProcessor.apvts, parameters::GAIN_PARAM_ID),
	osc(audioProcessor.apvts, parameters::OSCILLATOR_PARAM_ID),
	filter(audioProcessor.apvts, parameters::FILTER_TYPE_PARAM_ID, parameters::FILTER_CUTOFF_PARAM_ID, parameters::FILTER_RESONANCE_PARAM_ID),
	modAdsr("Mod Envelope", audioProcessor.apvts, parameters::MOD_ATTACK_PARAM_ID, parameters::MOD_DECAY_PARAM_ID, parameters::MOD_SUSTAIN_PARAM_ID, parameters::MOD_RELEASE_PARAM_ID)
{
	setSize(800, 600);
	addAndMakeVisible(presetPanel);

	addAndMakeVisible(adsr);
	addAndMakeVisible(gain);
	addAndMakeVisible(osc);
	addAndMakeVisible(filter);
	addAndMakeVisible(modAdsr);
}

SubSynthAudioProcessorEditor::~SubSynthAudioProcessorEditor() {}

void SubSynthAudioProcessorEditor::paint(juce::Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void SubSynthAudioProcessorEditor::resized()
{


	presetPanel.setBounds(getLocalBounds().removeFromTop(proportionOfWidth(0.1f)));

	auto height = getHeight() - presetPanel.getHeight();

	adsr.setBounds(0, presetPanel.getBottom(), getWidth() / 2, height / 5 * 2);
	gain.setBounds(0, adsr.getBottom(), getWidth() / 2, height / 5);
	modAdsr.setBounds(0, gain.getBottom(), getWidth() / 2, height / 5 * 2);

	osc.setBounds(gain.getRight(), presetPanel.getBottom(), getWidth() / 2, height / 2);
	filter.setBounds(osc.getX(), osc.getBottom(), getWidth() / 2, height / 2);
}
