/*
  ==============================================================================

	SynthVoice.cpp
	Created: 6 Nov 2025 2:26:57pm
	Author:  crist

  ==============================================================================
*/

#include "SynthVoice.h"

SynthVoice::SynthVoice()
{
}

SynthVoice::~SynthVoice()
{
}

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
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
	adsr.noteOff();
	modAdsr.noteOff();
}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{
	// gestione controller (se necessaria)
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

// Separare ADSR e GAIN ?
void SynthVoice::updateADSR(const float attack, const float decay, const float sustain, const float release, const float gainValue)
{
	adsr.updateADSR(attack, decay, sustain, release);
	gain.setGainLinear(gainValue);
}

void SynthVoice::updateFilter(int type, float cutoff, float resonance)
{
	// Memorizza i parametri base del filtro (senza consumare la mod envelope)
	filterType = type;
	filterCutoff = cutoff;
	filterResonance = resonance;

	// Aggiorna immediatamente tipo e risonanza (cutoff senza modulazione qui)
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
	jassert(isPrepared); // assicurati che la voce sia preparata prima del rendering

	const int numChannels = outputBuffer.getNumChannels();

	for (int i = 0; i < numSamples; ++i)
	{
		// Envelope amp e gain smussato
		const float envAmp = adsr.getNextSample();
		const float smoothedGain = gain.getNextSmoothedGain();

		// Oscillatore (mono) -> verrà duplicato sui canali
		const float oscSample = osc.processSample(0.0f);

		// Applica inviluppo di ampiezza
		float sample = oscSample * envAmp * smoothedGain;

		// Envelope di modulazione per il filtro (per-sample)
		const float mod = modAdsr.getNextSample(); // tipicamente [0..1]

		// Aggiorna i parametri del filtro usando la modulazione
		// Nota: qui mod scala la cutoff: cutoffEff = cutoffBase * mod
		filter.updateParameters(filterType, filterCutoff, filterResonance, mod);

		// Processa per canale con stato indipendente
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
	// gestione pitch wheel
}

