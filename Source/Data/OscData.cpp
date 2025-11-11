/*
  ==============================================================================

	OscData.cpp
	Created: 9 Nov 2025 4:43:23pm
	Author:  crist

  ==============================================================================
*/

#include "OscData.h"

void OscData::prepare(juce::dsp::ProcessSpec& spec)
{
	juce::dsp::Oscillator<float>::prepare(spec);
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

void OscData::setFrequency(const int midiNoteNumber)
{
	juce::dsp::Oscillator<float>::setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
}

float OscData::processSample(float input) {
	return juce::dsp::Oscillator<float>::processSample(input);
}

// ATTENZIONE: Fatto così nel tutorial, ma nel codice attuale non viene utilizzato (uso i sample e non gli AudioBlock)
// Forse dopo rinominare in process (come in GainData) per fare estensione del metodo originario
//void OscData::getNextAudioBlock(juce::dsp::AudioBlock<float>& block)
//{
//	process(juce::dsp::ProcessContextReplacing<float>(block));
//}