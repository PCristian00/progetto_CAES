/*
  ==============================================================================

    GainData.cpp
    Created: 8 Nov 2025 6:56:35pm
    Author:  crist

  ==============================================================================

*/

#include "GainData.h"

/**
 * Prepara il gain e la smussatura (SmoothedValue) con lo stesso ProcessSpec.
 * Imposta sample rate, prepara la base class e inizializza il valore smussato.
 *
 * @param spec ProcessSpec con sampleRate, maximumBlockSize, numChannels.
 */
void GainData::prepare(const juce::dsp::ProcessSpec& spec)
{
    currentSampleRate = spec.sampleRate;

    // prepara la classe base juce::dsp::Gain con lo stesso spec
    juce::dsp::Gain<float>::prepare(spec);

    // reset della SmoothedValue con sample rate e tempo di smoothing
    smoothedGain.reset(static_cast<float>(currentSampleRate), static_cast<float>(smoothingTimeSeconds));

    // inizializza valore corrente e target al valore attuale del gain della base class
    const float initial = static_cast<float>(juce::dsp::Gain<float>::getGainLinear());
    smoothedGain.setCurrentAndTargetValue(initial);
}

/**
 * Overload di prepare: costruisce il ProcessSpec e inoltra alla versione principale.
 *
 * @param newSampleRate sample rate.
 * @param samplesPerBlock dimensione massima del blocco.
 */
void GainData::prepare(double newSampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = newSampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.numChannels = 2;

    prepare(spec);
}

/**
 * Imposta il gain lineare come target per SmoothedValue e allinea la base class.
 *
 * @param newGain nuovo valore lineare del gain.
 */
void GainData::setGainLinear(const float newGain)
{
    // Aggiorna il target smussato
    smoothedGain.setTargetValue(newGain);

    // Manteniamo anche coerente lo stato della classe base (utile se usata altrove)
    juce::dsp::Gain<float>::setGainLinear(newGain);
}

/**
 * Restituisce il prossimo valore smussato del gain (per-sample).
 *
 * @return gain smussato corrente.
 */
float GainData::getNextSmoothedGain()
{
    return smoothedGain.getNextValue();
}
