/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Parameters.h"
#include <cmath>

SubSynthAudioProcessor::SubSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
	), apvts(*this, nullptr, "Parameters", parameters::createParameters())
#endif
{
	apvts.state.setProperty(Service::PresetManager::presetNameProperty, "", nullptr);
	apvts.state.setProperty("version", ProjectInfo::versionString, nullptr);

	presetManager = std::make_unique<Service::PresetManager>(apvts);

	synth.addSound(new SynthSound());

	// Inizializza il numero di voci dal parametro APVTS
	const int initialVoices = static_cast<int>(apvts.getRawParameterValue(parameters::NUM_VOICES)->load());
	updateSynthVoices(initialVoices);
}

SubSynthAudioProcessor::~SubSynthAudioProcessor() {}

/**
 * Nome del plugin.
 */
const juce::String SubSynthAudioProcessor::getName() const { return JucePlugin_Name; }

/**
 * Supporto input MIDI.
 */
bool SubSynthAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

/**
 * Supporto output MIDI.
 */
bool SubSynthAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

/**
 * Plugin di tipo effetto MIDI?
 */
bool SubSynthAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

/**
 * Tail length (nessuna coda).
 */
double SubSynthAudioProcessor::getTailLengthSeconds() const { return 0.0; }

/**
 * Programmi (non usati).
 */
int SubSynthAudioProcessor::getNumPrograms() { return 1; }
int SubSynthAudioProcessor::getCurrentProgram() { return 0; }
void SubSynthAudioProcessor::setCurrentProgram(int) {}
const juce::String SubSynthAudioProcessor::getProgramName(int) { return {}; }
void SubSynthAudioProcessor::changeProgramName(int, const juce::String&) {}

/**
 * Preparazione audio: imposta SR su synth/voices e prepara FX.
 *
 * @param sampleRate SR.
 * @param samplesPerBlock blocco.
 */
void SubSynthAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	lastSampleRate = sampleRate;
	lastBlockSize = samplesPerBlock;

    synth.setCurrentPlaybackSampleRate(sampleRate);
    for (int i = 0; i < synth.getNumVoices(); ++i)
        if (auto* voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voiceData.prepareVoice(*voice, sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }

	fx.prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());

    // Preparazione limiter
    limiter.prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
}

/**
 * Rilascio risorse (non necessario).
 */
void SubSynthAudioProcessor::releaseResources() {}

#ifndef JucePlugin_PreferredChannelConfigurations
/**
 * Supporto layout bus (mono/stereo).
 */
bool SubSynthAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	juce::ignoreUnused(layouts);
	return true;
#else
	if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
		layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
		return false;
#if ! JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif
	return true;
#endif
}
#endif

/**
 * Processo per blocco:
 * - Aggiorna numero voci da APVTS (runtime)
 * - Aggiorna parametri voce da APVTS
 * - Renderizza synth (dry)
 * - Aggiorna parametri FX e applica post-synth
 *
 * @param buffer audio.
 * @param midiMessages buffer MIDI.
 */
void SubSynthAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	juce::ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

	// Applica in tempo reale il numero di voci richiesto
	{
		const int desiredVoices = static_cast<int>(apvts.getRawParameterValue(parameters::NUM_VOICES)->load());
		updateSynthVoices(desiredVoices);
	}

	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	// Conta le voci attive
    int activeVoices = 0;
    for (int i = 0; i < synth.getNumVoices(); ++i)
        if (auto* v = synth.getVoice(i); v != nullptr && v->isVoiceActive())
            ++activeVoices;

    for (int i = 0; i < synth.getNumVoices(); ++i)
        if (auto* voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voiceData.applyParams(*voice, apvts, activeVoices);
        }

	// Render dry
	synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

	// Aggiorna FX dai parametri APVTS
	{
		const int fxType = static_cast<int>(apvts.getRawParameterValue(parameters::FX_TYPE)->load());
		const bool fxBypass = apvts.getRawParameterValue(parameters::FX_BYPASS)->load() > 0.5f;
		const float fxWet = apvts.getRawParameterValue(parameters::FX_WET)->load();

		fx.setType(static_cast<FXData::FxType>(fxType));
		fx.setBypass(fxBypass);
		fx.setWet(fxWet);

		// Chorus
		fx.setChorus(
			apvts.getRawParameterValue(parameters::CH_RATE)->load(),
			apvts.getRawParameterValue(parameters::CH_DEPTH)->load(),
			apvts.getRawParameterValue(parameters::CH_DELAY_MS)->load(),
			apvts.getRawParameterValue(parameters::CH_FEEDBACK)->load()
		);

		// Flanger
		fx.setFlanger(
			apvts.getRawParameterValue(parameters::FL_RATE)->load(),
			apvts.getRawParameterValue(parameters::FL_DEPTH)->load(),
			apvts.getRawParameterValue(parameters::FL_DELAY_MS)->load(),
			apvts.getRawParameterValue(parameters::FL_FEEDBACK)->load()
		);

		// Reverb
		fx.setReverb(
			apvts.getRawParameterValue(parameters::RV_SIZE)->load(),
			apvts.getRawParameterValue(parameters::RV_DAMP)->load(),
			apvts.getRawParameterValue(parameters::RV_WIDTH)->load()
		);
	}

	// Applica FX post-synth
	fx.process(buffer);

    // Aggiorna e applica limiter post chain
    {
        const float thrDb = apvts.getRawParameterValue(parameters::LIM_THRESHOLD_DB)->load();
        const float relMs = apvts.getRawParameterValue(parameters::LIM_RELEASE_MS)->load();
        limiter.setThreshold(thrDb);
        limiter.setRelease(relMs);
        limiter.process(buffer);
    }
}

/**
 * Editor supportato.
 */
bool SubSynthAudioProcessor::hasEditor() const { return true; }

/**
 * Crea l'editor principale.
 */
juce::AudioProcessorEditor* SubSynthAudioProcessor::createEditor() { return new SubSynthAudioProcessorEditor(*this); }

/**
 * Salvataggio stato plugin in XML, aggiungendo 'version'.
 */
void SubSynthAudioProcessor::getStateInformation(juce::MemoryBlock& destData) {
	auto state = apvts.copyState();

	// Assicura versione corrente prima di serializzare
	state.setProperty("version", ProjectInfo::versionString, nullptr);

	auto xml = state.createXml();
	copyXmlToBinary(*xml, destData);
}

/**
 * Ripristino stato da XML: garantisce proprieta' obbligatorie e rimpiazza lo stato.
 */
void SubSynthAudioProcessor::setStateInformation(const void* data, int sizeInBytes) {
	const auto xmlState = getXmlFromBinary(data, sizeInBytes);
	if (xmlState == nullptr)
		return;

	auto newTree = juce::ValueTree::fromXml(*xmlState);

	// Re-inserisci proprietà obbligatorie/migrate
	newTree.setProperty("version", ProjectInfo::versionString, nullptr);
	if (!newTree.hasProperty(Service::PresetManager::presetNameProperty))
		newTree.setProperty(Service::PresetManager::presetNameProperty, "", nullptr);

	apvts.replaceState(newTree);
}

/**
 * Factory del processor.
 */
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new SubSynthAudioProcessor(); }

/**
 * Aggiorna dinamicamente il numero di voci del synth in base al parametro.
 * - Aggiunge voci nuove preparandole con SR/blocco correnti.
 * - Rimuove voci in eccesso privilegiando quelle inattive (senza tagli udibili).
 */
void SubSynthAudioProcessor::updateSynthVoices(int desired)
{
	if (desired < 1) desired = 1;

	const int current = synth.getNumVoices();
	if (desired == current)
		return;

	if (desired > current)
	{
		const int toAdd = desired - current;
		for (int i = 0; i < toAdd; ++i)
		{
			auto* v = new SynthVoice();
			synth.addVoice(v);

            // Se prepareToPlay è già stato chiamato, prepara la nuova voce subito
            if (lastSampleRate > 0.0)
            {
                voiceData.prepareVoice(*v, lastSampleRate, lastBlockSize, getTotalNumOutputChannels());
            }
		}
	}
	else // desired < current
	{
		// Rimuovi dalla fine, provando a fermare prima la voce
		int toRemove = current - desired;
		for (int i = current - 1; i >= 0 && toRemove > 0; --i)
		{
			if (auto* v = synth.getVoice(i))
			{
				// Tenta di fermare immediatamente la voce
				v->stopNote(0.0f, false);

				// Rimuovi voce
				synth.removeVoice(i);
				--toRemove;
			}
		}
	}
}
