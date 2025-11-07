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

	void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);

	void updateADSR(const float attack, const float decay, const float sustain, const float release);
	void updateGain(const float gainValue);

	void renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
		int startSample,
		int numSamples) override;

private:

	juce::ADSR adsr;
	juce::ADSR::Parameters adsrParams;

	// 200 indica la LUT (controllare dopo)
	juce::dsp::Oscillator<float> osc{ [](float x) { return std::sin(x); 200;} };  //sin
	juce::dsp::Gain<float> gain;

	bool isPrepared{ false };

	// juce::dsp::Oscillator<float> osc{ [](float x) { return x / juce::MathConstants<float>::pi; } }; // saw
	// juce::dsp::Oscillator<float> osc{ [](float x) { return x < 0.0f ? -1.0f : 1.0f; } }; // square
};