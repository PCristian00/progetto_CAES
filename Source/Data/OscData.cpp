/*
  ==============================================================================

	OscData.cpp
	Created: 9 Nov 2025 4:43:23pm
	Author:  crist

  ==============================================================================
*/

#include "OscData.h"

/**
 * Prepara l'oscillatore principale e quello di modulazione FM.
 * Deve essere chiamata prima di generare qualunque sample.
 *
 * @param spec ProcessSpec con sampleRate, maximumBlockSize, numChannels.
 */
void OscData::prepare(juce::dsp::ProcessSpec& spec)
{
	fmOsc.prepare(spec);
	juce::dsp::Oscillator<float>::prepare(spec);
}

/**
 * Imposta la forma d'onda dell'oscillatore principale.
 * Indici:
 * 0 = Sine, 1 = Saw, 2 = Square, 3 = Triangle.
 *
 * @param choice indice forma d'onda (0..3).
 * In caso di indice non valido: jassertfalse.
 */
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
    case 3:
        // Triangle wave via asin(sin(x)) normalized to [-1, 1]
        initialise([](float x) { return 2.0f / juce::MathConstants<float>::pi * std::asin(std::sin(x)); });
        break;
	default:
		jassertfalse; // Unknown wave type!
		break;
	}
}

/**
 * Aggiorna la frequenza in base alla nota MIDI corrente (+ eventuale modulazione FM).
 * Conserva il numero di nota per riallineare la frequenza dopo cambi FM.
 *
 * @param midiNoteNumber numero di nota MIDI (0..127 tipicamente).
 */
void OscData::setFrequency(const int midiNoteNumber)
{
	const float baseHz = static_cast<float>(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
	const float currentFreq = baseHz + fmMod;
	juce::dsp::Oscillator<float>::setFrequency(currentFreq >= 0.0f ? currentFreq : -currentFreq);
	lastMidiNote = midiNoteNumber;
}

/**
 * Genera un singolo sample dall'oscillatore applicando modulazione FM.
 * La modulazione modifica la frequenza istantanea tramite fmOsc.
 *
 * @param input sample di tempo (tipicamente ignorato dall'oscillatore di JUCE, spesso 0.0f).
 * @return sample generato dall'oscillatore principale.
 */
float OscData::processSample(float input)
{
	fmMod = fmOsc.processSample(input) * fmDepth; // Modulation depth
	return juce::dsp::Oscillator<float>::processSample(input);
}

/**
 * Imposta i parametri della modulazione FM (frequenza e profondità) e riallinea la frequenza totale.
 *
 * @param depth profondità modulazione (scalare applicato al segnale di fmOsc).
 * @param freq  frequenza in Hz dell'oscillatore FM.
 */
void OscData::setFmParams(const float depth, const float freq)
{
	fmOsc.setFrequency(freq);
	fmDepth = depth;
	setFrequency(lastMidiNote);
}