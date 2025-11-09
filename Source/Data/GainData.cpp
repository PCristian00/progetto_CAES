/*
  ==============================================================================

    GainData.cpp
    Created: 8 Nov 2025 6:56:35pm
    Author:  crist

  ==============================================================================

*/

#include "GainData.h"

// Preparazione usando ProcessSpec (chiamata fatta da SynthVoice::prepareToPlay)
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

// Compatibilità: prepara costruendo un ProcessSpec e inoltrando
void GainData::prepare(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.numChannels = 2;

    prepare(spec);
}

// Imposta il target; SmoothedValue effettuerà il ramp internamente
void GainData::setGainLinear(const float newGain)
{
    // Aggiorna il target smussato
    smoothedGain.setTargetValue(newGain);

    // Manteniamo anche coerente lo stato della classe base (utile se usata altrove)
    juce::dsp::Gain<float>::setGainLinear(newGain);
}

float GainData::getNextSmoothedGain()
{
    return smoothedGain.getNextValue();
}
