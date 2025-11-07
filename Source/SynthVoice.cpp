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
	// in part1 <juce::SynthesiserSound*> (sound)
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

	// In futuro, vedere come collegare il gain al parametro del plugin
	gain.setGainLinear(0.1f);

	isPrepared = true;
}

void SynthVoice::updateADSR(const float attack, const float decay, const float sustain, const float release)
{
	adsrParams.attack = attack;
	adsrParams.decay = decay;
	adsrParams.sustain = sustain;
	adsrParams.release = release;
	adsr.setParameters(adsrParams);
}


// part 2
void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
	int startSample,
	int numSamples)
{
	// Implementation for rendering the next audio block
	jassert(isPrepared);

	juce::dsp::AudioBlock<float> audioBlock{ outputBuffer };
	osc.process(juce::dsp::ProcessContextReplacing<float>{ audioBlock });
	gain.process(juce::dsp::ProcessContextReplacing<float>{ audioBlock });

	adsr.applyEnvelopeToBuffer(outputBuffer, startSample, numSamples);
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{
	// Implementation for handling pitch wheel movements
}

