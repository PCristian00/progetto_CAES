/*
  ==============================================================================

	OscData.cpp
	Created: 9 Nov 2025 4:43:23pm
	Author:  crist

  ==============================================================================
*/

#include "OscData.h"

// Forse dopo rinominare in prepare (come in GainData) per fare estensione del metodo originario
// VALE PER TUTTI I METODI SOTTOSTANTI

void OscData::prepareToPlay(juce::dsp::ProcessSpec& spec)
{
	prepare(spec);
}

void OscData::setWaveType(const int choice)
{
	switch (choice)
	{
	case 0:
		initialise([](float x) { return std::sin(x); }); // Sine wave
		break;
	case 1:
		initialise([](float x) { return x / juce::MathConstants<float>::pi; }); // Saw wave
		break;
	case 2:
		initialise([](float x) { return x < 0.0f ? -1.0f : 1.0f; }); // Square wave
		break;
	default:
		jassertfalse; // Unknown wave type!
		break;
	}
}

void OscData::setWaveFrequency(const int midiNoteNumber)
{
	setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
}

// ATTENZIONE: Fatto così nel tutorial, ma nel codice attuale non viene utilizzato (uso i sample e non gli AudioBlock)
// Forse dopo rinominare in process (come in GainData) per fare estensione del metodo originario
void OscData::getNextAudioBlock(juce::dsp::AudioBlock<float>& block)
{
	process(juce::dsp::ProcessContextReplacing<float>(block));
}

// Forse rifare funzione custom anche per sample