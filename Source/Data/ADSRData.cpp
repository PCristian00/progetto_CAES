/*
  ==============================================================================

	ADSRData.cpp
	Created: 8 Nov 2025 4:16:15pm
	Author:  crist

  ==============================================================================
*/

#include "ADSRData.h"

/**
 * Aggiorna i parametri dell'ADSR e li applica al processore.
 *
 * @param attack Attacco.
 * @param decay Decadimento.
 * @param sustain Sostegno.
 * @param release Rilascio.
 */
void ADSRData::update(const float attack, const float decay, const float sustain, const float release)
{
	adsrParams.attack = attack;
	adsrParams.decay = decay;
	adsrParams.sustain = sustain;
	adsrParams.release = release;
	setParameters(adsrParams);
}