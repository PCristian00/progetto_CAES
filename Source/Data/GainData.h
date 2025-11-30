/*
  ==============================================================================

	GainData.h
	Created: 8 Nov 2025 6:56:35pm
	Author:  crist

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class GainData : public juce::dsp::Gain<float>
{
public:
	// Preparazione: overload che accetta il ProcessSpec (usato da juce::dsp)
	void prepare(const juce::dsp::ProcessSpec& spec);

	// Compatibilita': overload che accetta sampleRate e samplesPerBlock
	void prepare(double sampleRate, int samplesPerBlock);

	// Imposta il target gain (verrà applicato con smoothing)
	void setGainLinear(const float newGain);

	// Restituisce il valore smussato per il prossimo sample
	float getNextSmoothedGain();

	// Opzionale: cambia il tempo di smoothing (in secondi)
	void setSmoothingTimeSeconds(double seconds) { smoothingTimeSeconds = seconds; }

private:
	juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedGain;
	double currentSampleRate{ 44100.0 };
	double smoothingTimeSeconds{ 0.02 }; // default 20 ms
};
