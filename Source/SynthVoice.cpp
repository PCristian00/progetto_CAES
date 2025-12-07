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

/**
 * Indica se la voce puo' riprodurre il suono fornito.
 * Per ora accetta qualunque SynthesiserSound.
 */
bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
	return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

/**
 * Avvia una nota: imposta la frequenza dell'oscillatore e attiva gli inviluppi.
 *
 * @param midiNoteNumber numero di nota MIDI.
 * @param velocity velocita'/ampiezza iniziale.
 * @param sound suono (non usato).
 * @param currentPitchWheelPosition pitch wheel (non usato).
 */
void SynthVoice::startNote(int midiNoteNumber,
	float velocity,
	juce::SynthesiserSound* sound,
	int currentPitchWheelPosition)
{
    juce::ignoreUnused(sound, currentPitchWheelPosition, velocity);

	osc.setFrequency(midiNoteNumber);
	adsr.noteOn();
	modAdsr.noteOn();
}

/**
 * Ferma una nota: rilascia gli inviluppi (tail-off se consentito).
 *
 * @param velocity velocita' di rilascio.
 * @param allowTailOff consente coda naturale dell'inviluppo.
 */
void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    juce::ignoreUnused(velocity);
	adsr.noteOff();
	modAdsr.noteOff();

	// Se non e' consentito il tail-off, interrompi immediatamente la nota
	if (!allowTailOff)
	{
		clearCurrentNote();
		adsr.reset();
		modAdsr.reset();
	}
}

/**
 * Controller MIDI (non usato).
 */
void SynthVoice::controllerMoved(int, int) {}

/**
 * Prepara la voce: osc, inviluppi, filtro e gain.
 *
 * @param sampleRate SR del playback.
 * @param samplesPerBlock dimensione blocco.
 * @param outputChannels numero canali in uscita.
 */
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
}

/**
 * Aggiorna i parametri di un inviluppo ADSR (amp o mod) passato.
 *
 * @param attack A.
 * @param decay D.
 * @param sustain S.
 * @param release R.
 */
void SynthVoice::updateADSR(ADSRData& target, const float attack, const float decay, const float sustain, const float release)
{
	target.update(attack, decay, sustain, release);
}

/**
 * Imposta il gain lineare smussato.
 */
void SynthVoice::setGainLinear(const float gainValue)
{
	gain.setGainLinear(gainValue);
}

/**
 * Imposta il gain lineare applicando uno scaling polifonico (1/sqrt(n)) se n>0.
 */
void SynthVoice::setGainLinear(const float baseGainValue, int activeVoices)
{
	float scale = 1.0f;
	if (activeVoices > 0)
		scale = 1.0f / std::sqrt(static_cast<float>(activeVoices));
	gain.setGainLinear(baseGainValue * scale);
}

/**
 * Aggiorna i parametri base del filtro (tipo, cutoff, risonanza).
 * La modulazione verra' applicata per-sample in renderNextBlock.
 *
 * @param type tipo filtro.
 * @param cutoff cutoff in Hz.
 * @param resonance risonanza (Q).
 */
void SynthVoice::updateFilter(int type, float cutoff, float resonance)
{
	filterType = type;
	filterCutoff = cutoff;
	filterResonance = resonance;

	filter.updateParameters(filterType, filterCutoff, filterResonance, 1.0f);
}

/**
 * Genera il blocco di samples audio.
 *
 * @param outputBuffer buffer di output (multi-canale).
 * @param startSample indice del primo sample da scrivere.
 * @param numSamples numero di samples da generare.
 */
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

		const float mod = modAdsr.getNextSample();

		filter.updateParameters(filterType, filterCutoff, filterResonance, mod);

		for (int channel = 0; channel < numChannels; ++channel)
		{
			const float filtered = filter.processSample(channel, sample);
			outputBuffer.addSample(channel, startSample, filtered);
		}
		++startSample;
	}

	// Se gli inviluppi hanno terminato, libera la voce
	if (!adsr.isActive() && !modAdsr.isActive())
	{
		clearCurrentNote();
	}
}

/**
 * Pitch wheel (non gestito).
 */
void SynthVoice::pitchWheelMoved(int) {}

