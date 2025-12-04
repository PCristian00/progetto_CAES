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
	limiter(audioProcessor.apvts, parameters::LIM_THRESHOLD_DB, parameters::LIM_RELEASE_MS),
	osc(audioProcessor.apvts, parameters::OSCILLATOR_TYPE),
	filter(audioProcessor.apvts, parameters::FILTER_TYPE, parameters::FILTER_CUTOFF, parameters::FILTER_RESONANCE),
	modAdsr("Mod Envelope", audioProcessor.apvts, parameters::MOD_ATTACK, parameters::MOD_DECAY, parameters::MOD_SUSTAIN, parameters::MOD_RELEASE),
	fx(audioProcessor.apvts),
	voice(audioProcessor.apvts, parameters::NUM_VOICES)
{
	setSize(1080, 900);
	addAndMakeVisible(presetPanel);

	addAndMakeVisible(adsr);
	addAndMakeVisible(gain);
	addAndMakeVisible(limiter);
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

	const int panelBottom = presetPanel.getBottom();
	const int totalWidth = getWidth();
	const int totalHeightBelow = getHeight() - panelBottom;
	const int colWidth = totalWidth / 3;

	// Colonna sinistra: ADSR e Mod ADSR
	{
		const int x = 0;
		const int y = panelBottom;
		const int h = totalHeightBelow / 2;
		adsr.setBounds(x, y, colWidth, h);
		modAdsr.setBounds(x, adsr.getBottom(), colWidth, h);
	}

	// Colonna centrale: Oscillatore e Filtro
	{
		const int x = colWidth;
		const int y = panelBottom;
		const int h = totalHeightBelow / 2;
		osc.setBounds(x, y, colWidth, h);
		filter.setBounds(x, osc.getBottom(), colWidth, h);
	}

	// Colonna destra: FX, Gain, Voices, Limiter
	{
		const int x = colWidth * 2;
		const int y = panelBottom;
		const int h = totalHeightBelow / 2;

		fx.setBounds(x, y, colWidth, h);

		// Area rimanente sotto FX
		const int remainingY = fx.getBottom();
		const int remainingH = h / 2;

		gain.setBounds(x, remainingY, colWidth, remainingH);

		const int bottomRowY = gain.getBottom();
		const int halfW = colWidth / 2;
		voice.setBounds(x, bottomRowY, halfW, remainingH);
		limiter.setBounds(x + halfW, bottomRowY, halfW, remainingH);
	}
}
