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

	// Toggle debug
	void setModEnvelopeDebug(bool enabled) { debugModEnvEnabled = enabled; }
	void setAmpEnvelopeDebug(bool enabled) { debugAmpEnvEnabled = enabled; }

	// Imposta ogni quanti blocchi stampare (>=1)
	void setEnvelopeDebugRates(int ampEveryBlocks, int modEveryBlocks)
	{
		if (ampEveryBlocks > 0) ampPrintEvery = ampEveryBlocks;
		if (modEveryBlocks > 0) modPrintEvery = modEveryBlocks;
	}

private:

	bool isPrepared{ false };

	GainData gain;
	OscData osc;
	ADSRData adsr;     // amp envelope
	FilterData filter;
	ADSRData modAdsr;  // filter envelope

	int   filterType{ 0 };
	float filterCutoff{ 20000.0f };
	float filterResonance{ 0.7f };

	// Debug Mod Envelope
	bool  debugModEnvEnabled{ false };
	float lastBlockModMin{ 1.0f };
	float lastBlockModMax{ 0.0f };
	float lastEffectiveCutoff{ 0.0f };
	int   modBlockCounter{ 0 };
	int   modPrintEvery{ 30 }; // stampa ogni 30 blocchi (configurabile)

	// Debug Amp Envelope
	bool  debugAmpEnvEnabled{ false };
	float lastBlockAmpMin{ 1.0f };
	float lastBlockAmpMax{ 0.0f };
	float lastAppliedAmpPeak{ 0.0f };
	int   ampBlockCounter{ 0 };
	int   ampPrintEvery{ 30 }; // stampa ogni 30 blocchi (configurabile)
};