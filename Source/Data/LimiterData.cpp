/*
  ==============================================================================

	LimiterData.cpp
	Created: 4 Dec 2025 5:35:38pm
	Author:  crist

  ==============================================================================
*/

#include "LimiterData.h"

/*Prepara il limiter per la riproduzione.*/
void LimiterData::prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels)
{
	juce::dsp::ProcessSpec spec;
	spec.sampleRate = sampleRate;
	spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
	spec.numChannels = static_cast<juce::uint32>(numChannels);

	limiter.reset();
	limiter.prepare(spec);

	// default values
	limiter.setThreshold(-0.5f); // dB
	limiter.setRelease(50.0f);   // ms

	isPrepared = true;
}

/*Resetta il limiter.*/
void LimiterData::reset()
{
	limiter.reset();
}

/*Processa il buffer audio con il limiter.*/
void LimiterData::process(juce::AudioBuffer<float>& buffer)
{
	jassert(isPrepared);
	juce::dsp::AudioBlock<float> block(buffer);
	juce::dsp::ProcessContextReplacing<float> ctx(block);
	limiter.process(ctx);
}
