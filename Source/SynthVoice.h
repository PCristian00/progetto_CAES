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
#include "Data/OscData.h"
#include "Data/FilterData.h"

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
	void updateADSR(const float attack, const float decay, const float sustain, const float release, const float gainValue);
	void updateFilter(int filterType, float cutoff, float resonance);
	void updateModADSR(const float attack, const float decay, const float sustain, const float release);
	OscData& getOscillator() { return osc; }
	void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

private:

	bool isPrepared{ false };
	
	GainData gain;
	OscData osc;

	ADSRData adsr;     // amp envelope
	FilterData filter;
	ADSRData modAdsr;  // filter envelope

	// Parametri base del filtro (aggiornati dall'UI)
	int   filterType { 0 };
	float filterCutoff { 20000.0f };
	float filterResonance { 0.7f };
};