/*
  ==============================================================================

	SynthVoice.cpp
	Created: 6 Nov 2025 2:26:57pm
	Author:  crist

  ==============================================================================

*/

#include "SynthVoice.h"

SynthVoice::SynthVoice()
{

}

SynthVoice::~SynthVoice()
{

}

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
	return dynamic_cast<juce::SynthesiserSound*> (sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber,
	float velocity,
	juce::SynthesiserSound* sound,
	int currentPitchWheelPosition)
{
	osc.setFrequency(midiNoteNumber);
	adsr.noteOn();
	modAdsr.noteOn();
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
	adsr.noteOff();
	modAdsr.noteOff();
}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{
	// gestione controller (se necessaria)
}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{


	juce::dsp::ProcessSpec spec;
	spec.sampleRate = sampleRate;
	spec.maximumBlockSize = samplesPerBlock;
	spec.numChannels = static_cast<juce::uint32> (outputChannels);

	osc.prepare(spec);
	adsr.setSampleRate(sampleRate);
	filter.prepareToPlay(sampleRate, samplesPerBlock, outputChannels);
	modAdsr.setSampleRate(sampleRate);
	// Chiama l'overload che accetta ProcessSpec (definito in GainData)
	gain.prepare(spec);

	isPrepared = true;
}

// Separare ADSR e GAIN ?
void SynthVoice::updateADSR(const float attack, const float decay, const float sustain, const float release, const float gainValue)
{
	adsr.updateADSR(attack, decay, sustain, release);

	// Imposta il target del gain; GainData userà SmoothedValue internamente
	gain.setGainLinear(gainValue);
}

void SynthVoice::updateFilter(int filterType, float cutoff, float resonance)
{
	float modulator = modAdsr.getNextSample();
	filter.updateParameters(filterType, cutoff, resonance, modulator);
}

void SynthVoice::updateModADSR(const float attack, const float decay, const float sustain, const float release)
{
	modAdsr.updateADSR(attack, decay, sustain, release);
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
	int startSample,
	int numSamples)
{
	/*if (!isPrepared)
		return;*/

	jassert(isPrepared); // assicurati che la voce sia preparata prima del rendering

	const int numChannels = outputBuffer.getNumChannels();

	// CAPIRE COME METTERE ANCHE QUESTO
	// filter.process(synthBuffer);

	for (int i = 0; i < numSamples; ++i)
	{
		// ottieni il gain smussato per questo sample
		const float smoothedGain = gain.getNextSmoothedGain();

		// osc.getNextAudioBlock(block); // vecchia implementazione per blocchi

		const float sample = osc.processSample(0.0f) * adsr.getNextSample() * smoothedGain;

		// Capire come applicare al filtro
		// modAdsr.applyEnvelopeToBuffer(synthBuffer, 0, numSamples);

		for (int channel = 0; channel < numChannels; ++channel)
		{
			outputBuffer.addSample(channel, startSample, sample);
		}
		++startSample;

		// da tutorial
		// modAdsr.getNextSample();


		// filter.updateParameters(filterType, filterCutOff, filterResonance);

		
	}
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{
	// gestione pitch wheel
}

