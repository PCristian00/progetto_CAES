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
#include "Data/ADSRData.h"
#include "Data/GainData.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:

	SynthVoice();
	~SynthVoice() override;
	bool canPlaySound(juce::SynthesiserSound* sound) override;
	void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
	void stopNote(float velocity, bool allowTailOff) override;
	void pitchWheelMoved(int newPitchWheelValue) override;
	void controllerMoved(int controllerNumber, int newControllerValue) override;
	void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
	void update(const float attack, const float decay, const float sustain, const float release, const float gainValue);
	void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

private:

	bool isPrepared{ false };
	ADSRData adsr;
	GainData gain;

	// 200 indica la LUT (controllare dopo)
	juce::dsp::Oscillator<float> osc{ [](float x) { return std::sin(x); 200;} };  //sin
	
	// Vecchia implementazione diretta del gain (senza GainData)
	//juce::dsp::Gain<float> gain;	

	// juce::dsp::Oscillator<float> osc{ [](float x) { return x / juce::MathConstants<float>::pi; } }; // saw
	// juce::dsp::Oscillator<float> osc{ [](float x) { return x < 0.0f ? -1.0f : 1.0f; } }; // square
};