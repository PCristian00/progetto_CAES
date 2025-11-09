/*
  ==============================================================================

    GainData.cpp
    Created: 8 Nov 2025 6:56:35pm
    Author:  crist

  ==============================================================================
*/

#include "GainData.h"

// estensione di juce::dsp::Gain<float>::setGainLinear
void GainData::setGainLinear(const float newGain)
{
    juce::dsp::Gain<float>::setGainLinear(newGain);

    // Aggiungere eventuali funzioni aggiuntive OPPURE RIMUOVERE DEL TUTTO (già implementata per ereditarietà)
}
