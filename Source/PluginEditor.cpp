/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Parameters.h"

//==============================================================================
/**
 * Costruisce l'editor: crea pannelli e attacca controlli agli APVTS params.
 * Imposta dimensioni e aggiunge i componenti visibili.
 */
SubSynthAudioProcessorEditor::SubSynthAudioProcessorEditor(SubSynthAudioProcessor& p)
	: AudioProcessorEditor(&p),
	audioProcessor(p),
	presetPanel(p.getPresetManager()),
	adsr("Amp Envelope", audioProcessor.apvts, parameters::ATTACK, parameters::DECAY, parameters::SUSTAIN, parameters::RELEASE),
	gain(audioProcessor.apvts, parameters::GAIN),
	osc(audioProcessor.apvts, parameters::OSCILLATOR_TYPE),
	filter(audioProcessor.apvts, parameters::FILTER_TYPE, parameters::FILTER_CUTOFF, parameters::FILTER_RESONANCE),
	modAdsr("Mod Envelope", audioProcessor.apvts, parameters::MOD_ATTACK, parameters::MOD_DECAY, parameters::MOD_SUSTAIN, parameters::MOD_RELEASE),
	fx(audioProcessor.apvts),
	voice(audioProcessor.apvts, parameters::NUM_VOICES)
{
	setSize(900, 600);
	addAndMakeVisible(presetPanel);

	addAndMakeVisible(adsr);
	addAndMakeVisible(gain);
	addAndMakeVisible(osc);
	addAndMakeVisible(filter);
	addAndMakeVisible(modAdsr);
	addAndMakeVisible(fx);
	addAndMakeVisible(voice);
}

SubSynthAudioProcessorEditor::~SubSynthAudioProcessorEditor() {}

/**
 * Sfondo editor.
 */
void SubSynthAudioProcessorEditor::paint(juce::Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

/**
 * Layout dei componenti: suddivide in 3 colonne e piu' righe.
 */
void SubSynthAudioProcessorEditor::resized()
{
	presetPanel.setBounds(getLocalBounds().removeFromTop(proportionOfHeight(0.1f)));

	auto height = getHeight() - presetPanel.getHeight();

	adsr.setBounds(0, presetPanel.getBottom(), getWidth() / 3, height / 6 * 2);
	gain.setBounds(0, adsr.getBottom(), getWidth() / 3, height / 6);
	voice.setBounds(0, gain.getBottom(), getWidth() / 3, height / 6);
	modAdsr.setBounds(0, voice.getBottom(), getWidth() / 3, height / 6 * 2);

	osc.setBounds(gain.getRight(), presetPanel.getBottom(), getWidth() / 3, height / 2);
	filter.setBounds(osc.getX(), osc.getBottom(), getWidth() / 3, height / 2);
	fx.setBounds(filter.getRight(), presetPanel.getBottom(), getWidth() / 3, height);


}
