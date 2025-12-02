/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Parameters.h"

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
	synth.addVoice(new SynthVoice());
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
	synth.setCurrentPlaybackSampleRate(sampleRate);
	for (int i = 0; i < synth.getNumVoices(); ++i)
		if (auto* voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
		{
			voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
			voice->setAmpEnvelopeDebug(false);
			voice->setModEnvelopeDebug(false);
			voice->setEnvelopeDebugRates(60, 120);
		}

	fx.prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
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

	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	for (int i = 0; i < synth.getNumVoices(); ++i)
		if (auto* voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
		{
			float attack = apvts.getRawParameterValue(parameters::ATTACK)->load();
			float decay = apvts.getRawParameterValue(parameters::DECAY)->load();
			float sustain = apvts.getRawParameterValue(parameters::SUSTAIN)->load();
			float release = apvts.getRawParameterValue(parameters::RELEASE)->load();
			float gain = apvts.getRawParameterValue(parameters::GAIN)->load();
			const int oscChoice = static_cast<int>(apvts.getRawParameterValue(parameters::OSCILLATOR_TYPE)->load());
			float fmFreq = apvts.getRawParameterValue(parameters::FM_FREQUENCY)->load();
			float fmDepth = apvts.getRawParameterValue(parameters::FM_DEPTH)->load();
			float filterType = apvts.getRawParameterValue(parameters::FILTER_TYPE)->load();
			float filterCutOff = apvts.getRawParameterValue(parameters::FILTER_CUTOFF)->load();
			float filterResonance = apvts.getRawParameterValue(parameters::FILTER_RESONANCE)->load();
			float modAttack = apvts.getRawParameterValue(parameters::MOD_ATTACK)->load();
			float modDecay = apvts.getRawParameterValue(parameters::MOD_DECAY)->load();
			float modSustain = apvts.getRawParameterValue(parameters::MOD_SUSTAIN)->load();
			float modRelease = apvts.getRawParameterValue(parameters::MOD_RELEASE)->load();

			voice->getOscillator().setWaveType(oscChoice);
			voice->getOscillator().setFmParams(fmDepth, fmFreq);
			voice->updateADSR(attack, decay, sustain, release, gain);
			voice->updateFilter(static_cast<int>(filterType), filterCutOff, filterResonance);
			voice->updateModADSR(modAttack, modDecay, modSustain, modRelease);
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
