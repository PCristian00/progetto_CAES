/*
  ==============================================================================

	FilterData.cpp
	Created: 14 Nov 2025 3:54:10pm
	Author:  crist

  ==============================================================================
*/

#include "FilterData.h"

void FilterData::prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels)
{
	filter.reset();

	juce::dsp::ProcessSpec spec;
	spec.sampleRate = sampleRate;
	spec.maximumBlockSize = samplesPerBlock;
	spec.numChannels = static_cast<juce::uint32>(numChannels);
	filter.prepare(spec);

	isPrepared = true;
}

void FilterData::process(juce::AudioBuffer<float>& buffer)
{
	jassert(isPrepared); // make sure the filter is prepared before processing

	juce::dsp::AudioBlock<float> block(buffer);
	juce::dsp::ProcessContextReplacing<float> context(block);
	filter.process(context);
}

float FilterData::processSample(int channel, float inputSample)
{
	jassert(isPrepared);
	return filter.processSample(channel, inputSample);
}

void FilterData::updateParameters(int filterType, float cutoffFreq, float resonance, float modulator)
{
	switch (filterType)
	{
	case 0:
		filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
		break;
	case 1:
		filter.setType(juce::dsp::StateVariableTPTFilterType::bandpass);
		break;
	case 2:
		filter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
		break;
	}

	float modFreq = cutoffFreq * modulator;
	modFreq = std::fmin(std::fmax(modFreq, 20.0f), 20000.0f);

	filter.setCutoffFrequency(modFreq);
	filter.setResonance(resonance);
}

void FilterData::reset()
{
	filter.reset();
}
