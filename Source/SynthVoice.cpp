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
	return dynamic_cast<juce::SynthesiserSound*> (sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber,
	float velocity,
	juce::SynthesiserSound* sound,
	int currentPitchWheelPosition)
{
	osc.setFrequency(midiNoteNumber);
	adsr.noteOn();
	modAdsr.noteOn();
	filter.reset(); // importante per evitare stato sporco tra note
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
	adsr.noteOff();
	modAdsr.noteOff();
}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{
}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
	juce::dsp::ProcessSpec spec;
	spec.sampleRate = sampleRate;
	spec.maximumBlockSize = samplesPerBlock;
	spec.numChannels = static_cast<juce::uint32> (outputChannels);

	osc.prepare(spec);
	adsr.setSampleRate(sampleRate);
	filter.prepareToPlay(sampleRate, samplesPerBlock, outputChannels);
	modAdsr.setSampleRate(sampleRate);
	gain.prepare(spec);

	isPrepared = true;
}

void SynthVoice::updateADSR(const float attack, const float decay, const float sustain, const float release, const float gainValue)
{
	adsr.updateADSR(attack, decay, sustain, release);
	gain.setGainLinear(gainValue);
}

void SynthVoice::updateFilter(int type, float cutoff, float resonance)
{
	// Memorizza parametri base (nessun consumo della mod envelope qui!)
	filterType = type;
	filterCutoff = cutoff;
	filterResonance = resonance;

	// Aggiorna tipo e risonanza subito; cutoff base senza modulazione
	filter.updateParameters(filterType, filterCutoff, filterResonance, 1.0f);
}

void SynthVoice::updateModADSR(const float attack, const float decay, const float sustain, const float release)
{
	modAdsr.updateADSR(attack, decay, sustain, release);
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
	int startSample,
	int numSamples)
{
	jassert(isPrepared);

	const int numChannels = outputBuffer.getNumChannels();

	for (int i = 0; i < numSamples; ++i)
	{
		const float envAmp = adsr.getNextSample();
		const float smoothedGain = gain.getNextSmoothedGain();

		const float oscSample = osc.processSample(0.0f);
		float sample = oscSample * envAmp * smoothedGain;

		// Envelope di modulazione per il filtro (per-sample)
		const float envMod = modAdsr.getNextSample(); // ~[0..1]

		// Mappa la cutoff: evita di azzerarla quando envMod è 0
		// esempio semplice: 10%..100% della cutoff base
		const float cutoffEff = juce::jlimit(20.0f, 20000.0f, filterCutoff * (0.1f + 0.9f * envMod));

		// Aggiorna SOLO la cutoff per-sample
		filter.setCutoffFrequency(cutoffEff);

		for (int channel = 0; channel < numChannels; ++channel)
		{
			const float filtered = filter.processSample(channel, sample);
			outputBuffer.addSample(channel, startSample, filtered);
		}
		++startSample;
	}
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{
}

