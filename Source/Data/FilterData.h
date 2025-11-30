/*
  ==============================================================================

	FilterData.h
	Created: 14 Nov 2025 3:54:10pm
	Author:  crist

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class FilterData
{
public:

	void prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels);
	void process(juce::AudioBuffer<float>& buffer);
	float processSample(int channel, float inputSample);

	// modulator è tipicamente [0..1] e scala la cutoff
	void updateParameters(int filterType, float cutoffFreq, float resonance, float modulator = 1.0f);
	void reset();

private:
	bool isPrepared{ false };
	juce::dsp::StateVariableTPTFilter<float> filter;
};
