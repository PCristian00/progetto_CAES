/*
  ==============================================================================

	FilterData.cpp
	Created: 14 Nov 2025 3:54:10pm
	Author:  crist

  ==============================================================================
*/

#include "FilterData.h"

/**
 * Prepara il filtro con lo spec fornito e resetta stato interno.
 *
 * @param sampleRate SR.
 * @param samplesPerBlock dimensione blocco.
 * @param numChannels numero canali.
 */
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

/**
 * Processa un buffer intero in-place.
 * Precondizione: prepareToPlay chiamato.
 */
void FilterData::process(juce::AudioBuffer<float>& buffer)
{
	jassert(isPrepared);

	juce::dsp::AudioBlock<float> block(buffer);
	juce::dsp::ProcessContextReplacing<float> context(block);
	filter.process(context);
}

/**
 * Processa un singolo sample (per canale) e ritorna il valore filtrato.
 */
float FilterData::processSample(int channel, float inputSample)
{
	jassert(isPrepared);
	return filter.processSample(channel, inputSample);
}

/**
 * Aggiorna i parametri del filtro in base al tipo e modulatore.
 * Limita la frequenza risultante tra 20 Hz e 20 kHz.
 *
 * @param filterType 0=LP,1=BP,2=HP.
 * @param cutoffFreq frequenza base.
 * @param resonance risonanza.
 * @param modulator fattore modulazione (scala la cutoff).
 */
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

/**
 * Reset dello stato del filtro.
 */
void FilterData::reset()
{
	filter.reset();
}
