/*
  ==============================================================================

    GainData.cpp
    Created: 8 Nov 2025 6:56:35pm
    Author:  crist

  ==============================================================================
*/

#include "GainData.h"

// Probabilmente superfluo
// Valutare ovverride piuttosto, o NON USARE DEL TUTTO GainData
void GainData::setGain(const float newGain) noexcept
{
    setGainLinear(newGain);
}
