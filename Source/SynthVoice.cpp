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
	// Implementation for starting a note
	osc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
	adsr.noteOn();
}
void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
	// Implementation for stopping a note
	adsr.noteOff();
}
void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{
	// Implementation for handling controller movements
}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{

	adsr.setSampleRate(sampleRate);


	juce::dsp::ProcessSpec spec;
	spec.sampleRate = sampleRate;
	spec.maximumBlockSize = samplesPerBlock;
	spec.numChannels = static_cast<juce::uint32> (outputChannels);

	osc.prepare(spec);
	gain.prepare(spec);

	isPrepared = true;
}

void SynthVoice::update(const float attack, const float decay, const float sustain, const float release, const float gainValue) {
	adsr.updateADSR(attack, decay, sustain, release);
	gain.setGainLinear(gainValue);
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
	int startSample,
	int numSamples)
{
	if (!isPrepared)
		return;

	// int writePos = startSample;

	for (int i = 0; i < numSamples; ++i)
	{
		// genera un sample mono, applica ADSR e gain
		const float sample = osc.processSample(0.0f) * adsr.getNextSample() * gain.getGainLinear();

		for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
		{
			outputBuffer.addSample(channel, startSample, sample);
		}
		++startSample;
	}
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{
	// Implementation for handling pitch wheel movements
}

