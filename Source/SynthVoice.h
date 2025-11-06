/*
  ==============================================================================

	SynthVoice.h
	Created: 6 Nov 2025 2:26:57pm
	Author:  crist

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SynthSound.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
	SynthVoice();
	~SynthVoice() override;
	bool canPlaySound(juce::SynthesiserSound* sound) override;
	void startNote(int midiNoteNumber,
		float velocity,
		juce::SynthesiserSound* sound,
		int currentPitchWheelPosition) override;
	void stopNote(float velocity, bool allowTailOff) override;
	void pitchWheelMoved(int newPitchWheelValue) override;
	void controllerMoved(int controllerNumber, int newControllerValue) override;
	void renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
		int startSample,
		int numSamples) override;
};