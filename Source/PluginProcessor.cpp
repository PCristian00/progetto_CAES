/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SubSynthAudioProcessor::SubSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
	), apvts(*this, nullptr, "Parameters", createParameters())
#endif
{
	synth.addSound(new SynthSound());
	synth.addVoice(new SynthVoice());

}

SubSynthAudioProcessor::~SubSynthAudioProcessor()
{
}

//==============================================================================
const juce::String SubSynthAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool SubSynthAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool SubSynthAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool SubSynthAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double SubSynthAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int SubSynthAudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
	// so this should be at least 1, even if you're not really implementing programs.
}

int SubSynthAudioProcessor::getCurrentProgram()
{
	return 0;
}

void SubSynthAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String SubSynthAudioProcessor::getProgramName(int index)
{
	return {};
}

void SubSynthAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
// part 0 del tutorial
void SubSynthAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{


	// part 1 del tutorial
	synth.setCurrentPlaybackSampleRate(sampleRate);

	for (int i = 0; i < synth.getNumVoices(); i++)
	{
		if (auto* voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
		{
			voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
		}
	}
}

void SubSynthAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SubSynthAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	juce::ignoreUnused(layouts);
	return true;
#else
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	// Some plugin hosts, such as certain GarageBand versions, will only
	// load plugins that support stereo bus layouts.
	if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
		return false;

	// This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif

	return true;
#endif
}
#endif

// part 0 del tutorial
void SubSynthAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	juce::ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	for (int i = 0; i < synth.getNumVoices(); i++)
	{
		// in part 1 <SynthesiserVoice*> (synth.getVoice(i))
		if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
		{

			// VEDERE part 5: fare funzione tipo UpdateGain in SynthVoice
			// voice->gain.setGainLinear(apvts.getRawParameterValue("GAIN")->load());
			// OSC Controls, ADSR...

			voice->updateGain(apvts.getRawParameterValue("GAIN")->load());

			voice->updateADSR(apvts.getRawParameterValue("ATTACK")->load(),
				apvts.getRawParameterValue("DECAY")->load(),
				apvts.getRawParameterValue("SUSTAIN")->load(),
				apvts.getRawParameterValue("RELEASE")->load());
		}
	}

	// part 1 del tutorial
	synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool SubSynthAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SubSynthAudioProcessor::createEditor()
{
	return new SubSynthAudioProcessorEditor(*this);
}

//==============================================================================
void SubSynthAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
}

void SubSynthAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new SubSynthAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout SubSynthAudioProcessor::createParameters()
{

	std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

	// Combobox: switch oscillator

	params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC","Oscillator",juce::StringArray{ "Sine", "Saw", "Square" },0));

	params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", // parameterID
		"Attack", // parameter name
		0.1f, // min value
		1.0f, // max value
		0.1f)); // default value

	params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", // parameterID
		"Decay", // parameter name
		0.1f, // min value
		1.0f, // max value
		0.1f)); // default value

	params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", // parameterID
		"Sustain", // parameter name
		0.1f, // min value
		1.0f, // max value
		1.0f)); // default value

	params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", // parameterID
		"Release", // parameter name
		0.1f, // min value
		3.0f, // max value
		0.4f)); // default value

	params.push_back(std::make_unique<juce::AudioParameterFloat>("GAIN", // parameterID
		"Gain", // parameter name
		0.0f, // min value
		1.0f, // max value
		0.5f)); // default value

	return { params.begin(), params.end() };

	//return layout;
}
