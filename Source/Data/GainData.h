/*
  ==============================================================================

    GainData.h
    Created: 8 Nov 2025 6:56:35pm
    Author:  crist

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>



// GainData forse non è utile, ma lo creo per coerenza con ADSRData
// Update di gain in precedenza era direttamente eseguito in SynthVoice.cpp
// Lascerò commentato il codice precedente in SynthVoice.cpp per riferimento futuro

class GainData : public juce::dsp::Gain<float>
{
public:
    void setGainLinear(const float newGain);
};
