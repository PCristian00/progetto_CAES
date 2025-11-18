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
	synth.addSound(new SynthSound());
	synth.addVoice(new SynthVoice());
}

SubSynthAudioProcessor::~SubSynthAudioProcessor() {}

const juce::String SubSynthAudioProcessor::getName() const { return JucePlugin_Name; }

bool SubSynthAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool SubSynthAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool SubSynthAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double SubSynthAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int SubSynthAudioProcessor::getNumPrograms() { return 1; }
int SubSynthAudioProcessor::getCurrentProgram() { return 0; }
void SubSynthAudioProcessor::setCurrentProgram(int) {}
const juce::String SubSynthAudioProcessor::getProgramName(int) { return {}; }
void SubSynthAudioProcessor::changeProgramName(int, const juce::String&) {}

void SubSynthAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	synth.setCurrentPlaybackSampleRate(sampleRate);
	for (int i = 0; i < synth.getNumVoices(); ++i)
	{
		if (auto* voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
		{
			voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
			voice->setAmpEnvelopeDebug(true);
			voice->setModEnvelopeDebug(true);
			voice->setEnvelopeDebugRates(60, 120);
		}
	}
}

void SubSynthAudioProcessor::releaseResources() {}

#ifndef JucePlugin_PreferredChannelConfigurations
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

void SubSynthAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	juce::ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	for (int i = 0; i < synth.getNumVoices(); ++i)
	{
		if (auto* voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
		{
			float attack = apvts.getRawParameterValue(parameters::ATTACK_PARAM_ID)->load();
			float decay = apvts.getRawParameterValue(parameters::DECAY_PARAM_ID)->load();
			float sustain = apvts.getRawParameterValue(parameters::SUSTAIN_PARAM_ID)->load();
			float release = apvts.getRawParameterValue(parameters::RELEASE_PARAM_ID)->load();
			float gain = apvts.getRawParameterValue(parameters::GAIN_PARAM_ID)->load();
			int oscChoice = apvts.getRawParameterValue(parameters::OSCILLATOR_PARAM_ID)->load();
			float fmFreq = apvts.getRawParameterValue(parameters::FM_FREQUENCY_PARAM_ID)->load();
			float fmDepth = apvts.getRawParameterValue(parameters::FM_DEPTH_PARAM_ID)->load();
			float filterType = apvts.getRawParameterValue(parameters::FILTER_TYPE_PARAM_ID)->load();
			float filterCutOff = apvts.getRawParameterValue(parameters::FILTER_CUTOFF_PARAM_ID)->load();
			float filterResonance = apvts.getRawParameterValue(parameters::FILTER_RESONANCE_PARAM_ID)->load();
			float modAttack = apvts.getRawParameterValue(parameters::MOD_ATTACK_PARAM_ID)->load();
			float modDecay = apvts.getRawParameterValue(parameters::MOD_DECAY_PARAM_ID)->load();
			float modSustain = apvts.getRawParameterValue(parameters::MOD_SUSTAIN_PARAM_ID)->load();
			float modRelease = apvts.getRawParameterValue(parameters::MOD_RELEASE_PARAM_ID)->load();

			voice->getOscillator().setWaveType(oscChoice); // impostazione tipo di onda
			voice->getOscillator().setFmParams(fmDepth, fmFreq); // impostazione FM (frequenza e profondità)
			voice->updateADSR(attack, decay, sustain, release, gain); // aggiornamento inviluppo ampiezza
			voice->updateFilter((int)filterType, filterCutOff, filterResonance); // aggiornamento filtro
			voice->updateModADSR(modAttack, modDecay, modSustain, modRelease); // aggiornamento inviluppo modulazione
		}
	}

	synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

bool SubSynthAudioProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor* SubSynthAudioProcessor::createEditor() { return new SubSynthAudioProcessorEditor(*this); }

void SubSynthAudioProcessor::getStateInformation(juce::MemoryBlock&) {}
void SubSynthAudioProcessor::setStateInformation(const void*, int) {}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new SubSynthAudioProcessor(); }
