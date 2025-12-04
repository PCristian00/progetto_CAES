/*
  ==============================================================================

	VoiceData.cpp
	Created: 4 Dec 2025 5:57:46pm
	Author:  crist

  ==============================================================================
*/

#include "VoiceData.h"
#include "../Parameters.h"

/*Prepara la voce per la riproduzione.*/
void VoiceData::prepareVoice(SynthVoice& voice, double sampleRate, int samplesPerBlock, int numOutputChannels)
{
	voice.prepareToPlay(sampleRate, samplesPerBlock, numOutputChannels);
	voice.setAmpEnvelopeDebug(false);
	voice.setModEnvelopeDebug(false);
	voice.setEnvelopeDebugRates(60, 120);
}

/*Applica i parametri della voce dal ValueTreeState. */
void VoiceData::applyParams(SynthVoice& voice,
	const juce::AudioProcessorValueTreeState& apvts,
	float polyGainScale)
{
	const float attack = apvts.getRawParameterValue(parameters::ATTACK)->load();
	const float decay = apvts.getRawParameterValue(parameters::DECAY)->load();
	const float sustain = apvts.getRawParameterValue(parameters::SUSTAIN)->load();
	const float release = apvts.getRawParameterValue(parameters::RELEASE)->load();
	const float gain = apvts.getRawParameterValue(parameters::GAIN)->load();

	const int oscChoice = static_cast<int>(apvts.getRawParameterValue(parameters::OSCILLATOR_TYPE)->load());
	const float fmFreq = apvts.getRawParameterValue(parameters::FM_FREQUENCY)->load();
	const float fmDepth = apvts.getRawParameterValue(parameters::FM_DEPTH)->load();

	const int  filterType = static_cast<int>(apvts.getRawParameterValue(parameters::FILTER_TYPE)->load());
	const float filterCutOff = apvts.getRawParameterValue(parameters::FILTER_CUTOFF)->load();
	const float filterResonance = apvts.getRawParameterValue(parameters::FILTER_RESONANCE)->load();

	const float modAttack = apvts.getRawParameterValue(parameters::MOD_ATTACK)->load();
	const float modDecay = apvts.getRawParameterValue(parameters::MOD_DECAY)->load();
	const float modSustain = apvts.getRawParameterValue(parameters::MOD_SUSTAIN)->load();
	const float modRelease = apvts.getRawParameterValue(parameters::MOD_RELEASE)->load();

	voice.getOscillator().setWaveType(oscChoice);
	voice.getOscillator().setFmParams(fmDepth, fmFreq);

	voice.updateADSR(attack, decay, sustain, release, gain * polyGainScale);
	voice.updateFilter(filterType, filterCutOff, filterResonance);
	voice.updateModADSR(modAttack, modDecay, modSustain, modRelease);
}
