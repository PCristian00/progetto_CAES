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
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
	adsr.noteOff();
}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{
	// gestione controller (se necessaria)
}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
	adsr.setSampleRate(sampleRate);

	juce::dsp::ProcessSpec spec;
	spec.sampleRate = sampleRate;
	spec.maximumBlockSize = samplesPerBlock;
	spec.numChannels = static_cast<juce::uint32> (outputChannels);

	osc.prepare(spec);

	// Chiama l'overload che accetta ProcessSpec (definito in GainData)
	gain.prepare(spec);

	isPrepared = true;
}

void SynthVoice::update(const float attack, const float decay, const float sustain, const float release, const float gainValue)
{
	adsr.updateADSR(attack, decay, sustain, release);

	// Imposta il target del gain; GainData userà SmoothedValue internamente
	gain.setGainLinear(gainValue);
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
	int startSample,
	int numSamples)
{
	if (!isPrepared)
		return;

	const int numChannels = outputBuffer.getNumChannels();

	for (int i = 0; i < numSamples; ++i)
	{
		// ottieni il gain smussato per questo sample
		const float smoothedGain = gain.getNextSmoothedGain();

		// osc.getNextAudioBlock(block); // vecchia implementazione per blocchi

		const float sample = osc.processSample(0.0f) * adsr.getNextSample() * smoothedGain;

		for (int channel = 0; channel < numChannels; ++channel)
		{
			outputBuffer.addSample(channel, startSample, sample);
		}
		++startSample;
	}
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{
	// gestione pitch wheel
}

