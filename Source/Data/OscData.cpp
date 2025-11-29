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
	fmOsc.prepare(spec);
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
    const float baseHz = static_cast<float>(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    const float currentFreq = baseHz + fmMod;
    juce::dsp::Oscillator<float>::setFrequency(currentFreq >= 0.0f ? currentFreq : -currentFreq);
    lastMidiNote = midiNoteNumber;
}

// Aggiunta modulazione FM.
// ATTENZIONE: metodo diverso dal tutorial, che usa AudioBlock invece di singoli sample
float OscData::processSample(float input) {

	fmMod = fmOsc.processSample(input) * fmDepth; // Modulation depth
	return juce::dsp::Oscillator<float>::processSample(input);
}

void OscData::setFmParams(const float depth, const float freq) {
	fmOsc.setFrequency(freq);
	fmDepth = depth;
	setFrequency(lastMidiNote);
}

// ATTENZIONE: Fatto cos nel tutorial, ma nel codice attuale non viene utilizzato (uso i sample e non gli AudioBlock)
// Forse dopo rinominare in process (come in GainData) per fare estensione del metodo originario
//void OscData::getNextAudioBlock(juce::dsp::AudioBlock<float>& block)
//{
//	process(juce::dsp::ProcessContextReplacing<float>(block));
//}