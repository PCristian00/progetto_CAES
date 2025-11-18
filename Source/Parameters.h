/*
  ==============================================================================

	Parameters.h
	Created: 17 Nov 2025 5:49:13pm
	Author:  crist

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

using String = juce::String;

namespace parameters
{
	const String OSCILLATOR_PARAM_ID = "OSC";
	const String FM_FREQUENCY_PARAM_ID = "FMFREQ";
	const String FM_DEPTH_PARAM_ID = "FMDEPTH";
	const String ATTACK_PARAM_ID = "ATTACK";
	const String DECAY_PARAM_ID = "DECAY";
	const String SUSTAIN_PARAM_ID = "SUSTAIN";
	const String RELEASE_PARAM_ID = "RELEASE";
	const String GAIN_PARAM_ID = "GAIN";
	const String FILTER_TYPE_PARAM_ID = "FILTERTYPE";
	const String FILTER_CUTOFF_PARAM_ID = "FILTERCUTOFF";
	const String FILTER_RESONANCE_PARAM_ID = "FILTERRESONANCE";
	const String MOD_ATTACK_PARAM_ID = "MODATTACK";
	const String MOD_DECAY_PARAM_ID = "MODDECAY";
	const String MOD_SUSTAIN_PARAM_ID = "MODSUSTAIN";
	const String MOD_RELEASE_PARAM_ID = "MODRELEASE";


	inline juce::AudioProcessorValueTreeState::ParameterLayout createParameters()
	{

		std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

		// Combobox: switch oscillator
		params.push_back(std::make_unique<juce::AudioParameterChoice>(OSCILLATOR_PARAM_ID, "Oscillator", juce::StringArray{ "Sine", "Saw", "Square" }, 0));
		// FM parameters
		params.push_back(std::make_unique<juce::AudioParameterFloat>(FM_FREQUENCY_PARAM_ID, "FM Frequency", juce::NormalisableRange<float>{0.0f, 1000.0f, 0.01f, 0.3f}, 5.0f));
		params.push_back(std::make_unique<juce::AudioParameterFloat>(FM_DEPTH_PARAM_ID, "FM Depth", juce::NormalisableRange<float>{0.0f, 1000.0f, 0.01f, 0.3f}, 0.0f));
		// Gain parameter
		params.push_back(std::make_unique<juce::AudioParameterFloat>(GAIN_PARAM_ID, "Gain", 0.0f, 1.0f, 0.5f));
		// ADSR parameters
		params.push_back(std::make_unique<juce::AudioParameterFloat>(ATTACK_PARAM_ID, "Attack", 0.1f, 1.0f, 0.1f));
		params.push_back(std::make_unique<juce::AudioParameterFloat>(DECAY_PARAM_ID, "Decay", 0.1f, 1.0f, 0.1f));
		params.push_back(std::make_unique<juce::AudioParameterFloat>(SUSTAIN_PARAM_ID, "Sustain", 0.1f, 1.0f, 1.0f));
		params.push_back(std::make_unique<juce::AudioParameterFloat>(RELEASE_PARAM_ID, "Release", 0.1f, 3.0f, 0.4f));
		// Filter parameters
		params.push_back(std::make_unique<juce::AudioParameterChoice>(FILTER_TYPE_PARAM_ID, "Filter Type", juce::StringArray{ "Low-Pass", "Band-Pass", "High-Pass" }, 0));
		params.push_back(std::make_unique<juce::AudioParameterFloat>(FILTER_CUTOFF_PARAM_ID, "Filter Cutoff", juce::NormalisableRange<float>{20.0f, 20000.0f, 0.1f, 0.6f}, 200.0f));
		params.push_back(std::make_unique<juce::AudioParameterFloat>(FILTER_RESONANCE_PARAM_ID, "Filter Resonance", juce::NormalisableRange<float>{1.0f, 10.0f, 0.1f}, 1.0f));
		// Filter ADSR parameters
		params.push_back(std::make_unique<juce::AudioParameterFloat>(MOD_ATTACK_PARAM_ID, "Mod Attack", 0.1f, 1.0f, 0.1f));
		params.push_back(std::make_unique<juce::AudioParameterFloat>(MOD_DECAY_PARAM_ID, "Mod Decay", 0.1f, 1.0f, 0.1f));
		params.push_back(std::make_unique<juce::AudioParameterFloat>(MOD_SUSTAIN_PARAM_ID, "Mod Sustain", 0.1f, 1.0f, 1.0f));
		params.push_back(std::make_unique<juce::AudioParameterFloat>(MOD_RELEASE_PARAM_ID, "Mod Release", 0.1f, 3.0f, 0.4f));

		return { params.begin(), params.end() };
	}

};