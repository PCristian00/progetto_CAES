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
}
void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
	// Implementation for stopping a note
}
void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{
	// Implementation for handling controller movements
}
void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
	int startSample,
	int numSamples)
{
	// Implementation for rendering the next audio block
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{
	// Implementation for handling pitch wheel movements
}

