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

	void prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels); // , int outputChannels);
	void process(juce::AudioBuffer<float>& buffer);
	void updateParameters(int filterType, float cutoffFreq, float resonance, float modulator = 1.0f);
	void reset();

	/*enum FilterType
	{
		lowPass,
		bandPass,
		highPass
	};
	FilterData();
	~FilterData();

	void updateFilter(FilterType type, float cutoffFreq, float resonance);
	;*/
private:
	/*juce::dsp::ProcessorChain<juce::dsp::StateVariableTPTFilter<float>, juce::dsp::Gain<float>> filterChain;
	*/
	bool isPrepared{ false };
	juce::dsp::StateVariableTPTFilter<float> filter;
};
