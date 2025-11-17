/*
  ==============================================================================

	SynthVoice.cpp
	Created: 6 Nov 2025 2:26:57pm
	Author:  crist

  ==============================================================================
*/

#include "SynthVoice.h"

SynthVoice::SynthVoice() {}
SynthVoice::~SynthVoice() {}

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
	return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber,
	float velocity,
	juce::SynthesiserSound* sound,
	int currentPitchWheelPosition)
{
	osc.setFrequency(midiNoteNumber);
	adsr.noteOn();
	modAdsr.noteOn();

	if (debugAmpEnvEnabled)
		DBG("[AmpADSR] startNote MIDI=" << midiNoteNumber << " vel=" << velocity);
	if (debugModEnvEnabled)
		DBG("[ModADSR] startNote MIDI=" << midiNoteNumber << " vel=" << velocity);
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
	adsr.noteOff();
	modAdsr.noteOff();

	if (debugAmpEnvEnabled)
		DBG("[AmpADSR] stopNote vel=" << velocity << " tailOff=" << (allowTailOff ? "true" : "false"));
	if (debugModEnvEnabled)
		DBG("[ModADSR] stopNote vel=" << velocity << " tailOff=" << (allowTailOff ? "true" : "false"));
}

void SynthVoice::controllerMoved(int, int) {}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
	juce::dsp::ProcessSpec spec;
	spec.sampleRate = sampleRate;
	spec.maximumBlockSize = samplesPerBlock;
	spec.numChannels = static_cast<juce::uint32>(outputChannels);

	osc.prepare(spec);
	adsr.setSampleRate(sampleRate);
	filter.prepareToPlay(sampleRate, samplesPerBlock, outputChannels);
	modAdsr.setSampleRate(sampleRate);
	gain.prepare(spec);

	isPrepared = true;

	if (debugAmpEnvEnabled)
		DBG("[AmpADSR] prepareToPlay sr=" << sampleRate << " block=" << samplesPerBlock);
	if (debugModEnvEnabled)
		DBG("[ModADSR] prepareToPlay sr=" << sampleRate << " block=" << samplesPerBlock);
}

void SynthVoice::updateADSR(const float attack, const float decay, const float sustain, const float release, const float gainValue)
{
	adsr.updateADSR(attack, decay, sustain, release);
	gain.setGainLinear(gainValue);

	if (debugAmpEnvEnabled)
		DBG("[AmpADSR] updateADSR A=" << attack << " D=" << decay << " S=" << sustain << " R=" << release << " Gain=" << gainValue);
}

void SynthVoice::updateFilter(int type, float cutoff, float resonance)
{
	filterType = type;
	filterCutoff = cutoff;
	filterResonance = resonance;

	filter.updateParameters(filterType, filterCutoff, filterResonance, 1.0f);

	if (debugModEnvEnabled)
		DBG("[ModADSR] BaseFilter type=" << filterType << " cutoff=" << filterCutoff << " res=" << filterResonance);
}

void SynthVoice::updateModADSR(const float attack, const float decay, const float sustain, const float release)
{
	modAdsr.updateADSR(attack, decay, sustain, release);

	if (debugModEnvEnabled)
		DBG("[ModADSR] updateModADSR A=" << attack << " D=" << decay << " S=" << sustain << " R=" << release);
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
	int startSample,
	int numSamples)
{
	jassert(isPrepared);

	const int numChannels = outputBuffer.getNumChannels();

	// Reset tracking per blocco
	if (debugModEnvEnabled)
	{
		lastBlockModMin = 1.0f;
		lastBlockModMax = 0.0f;
	}
	if (debugAmpEnvEnabled)
	{
		lastBlockAmpMin = 1.0f;
		lastBlockAmpMax = 0.0f;
		lastAppliedAmpPeak = 0.0f;
	}

	for (int i = 0; i < numSamples; ++i)
	{
		const float envAmp = adsr.getNextSample();
		const float smoothedGain = gain.getNextSmoothedGain();
		const float oscSample = osc.processSample(0.0f);
		float sample = oscSample * envAmp * smoothedGain;

		if (debugAmpEnvEnabled)
		{
			if (envAmp < lastBlockAmpMin) lastBlockAmpMin = envAmp;
			if (envAmp > lastBlockAmpMax) lastBlockAmpMax = envAmp;
			const float appliedAmp = envAmp * smoothedGain;
			if (appliedAmp > lastAppliedAmpPeak) lastAppliedAmpPeak = appliedAmp;
		}

		const float mod = modAdsr.getNextSample();
		if (debugModEnvEnabled)
		{
			if (mod < lastBlockModMin) lastBlockModMin = mod;
			if (mod > lastBlockModMax) lastBlockModMax = mod;
		}

		filter.updateParameters(filterType, filterCutoff, filterResonance, mod);

		for (int channel = 0; channel < numChannels; ++channel)
		{
			const float filtered = filter.processSample(channel, sample);
			outputBuffer.addSample(channel, startSample, filtered);
		}

		++startSample;
	}

	// Incrementa contatori blocchi
	ampBlockCounter++;
	modBlockCounter++;

	// Stampa solo ogni N blocchi
	if (debugAmpEnvEnabled && (ampBlockCounter % ampPrintEvery == 0))
	{
		DBG("[AmpADSR] Block#" << ampBlockCounter
			<< " envMin=" << lastBlockAmpMin
			<< " envMax=" << lastBlockAmpMax
			<< " appliedPeak=" << lastAppliedAmpPeak);
	}

	if (debugModEnvEnabled && (modBlockCounter % modPrintEvery == 0))
	{
		lastEffectiveCutoff = filterCutoff * lastBlockModMax;
		DBG("[ModADSR] Block#" << modBlockCounter
			<< " modMin=" << lastBlockModMin
			<< " modMax=" << lastBlockModMax
			<< " cutoffEffApprox=" << lastEffectiveCutoff);
	}
}

void SynthVoice::pitchWheelMoved(int) {}

