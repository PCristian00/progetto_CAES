/*
  ==============================================================================

    ADSRData.cpp
    Created: 8 Nov 2025 4:16:15pm
    Author:  crist

  ==============================================================================
*/

#include "ADSRData.h"

// FORSE RINOMINA IN update
void ADSRData::updateADSR(const float attack, const float decay, const float sustain, const float release)
{
	adsrParams.attack = attack;
	adsrParams.decay = decay;
	adsrParams.sustain = sustain;
	adsrParams.release = release;
	setParameters(adsrParams);
}	