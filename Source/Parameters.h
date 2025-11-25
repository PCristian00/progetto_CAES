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
	// Oscillator: IDs
	static constexpr const char* OSCILLATOR_PARAM_ID = "OSC";
	// FM: IDs
	static constexpr const char* FM_FREQUENCY_PARAM_ID = "FMFREQ";
	static constexpr const char* FM_DEPTH_PARAM_ID = "FMDEPTH";
	// ADSR: IDs
	static constexpr const char* ATTACK_PARAM_ID = "ATTACK";
	static constexpr const char* DECAY_PARAM_ID = "DECAY";
	static constexpr const char* SUSTAIN_PARAM_ID = "SUSTAIN";
	static constexpr const char* RELEASE_PARAM_ID = "RELEASE";
	static constexpr const char* GAIN_PARAM_ID = "GAIN";
	// Filter: IDs
	static constexpr const char* FILTER_TYPE_PARAM_ID = "FILTERTYPE";
	static constexpr const char* FILTER_CUTOFF_PARAM_ID = "FILTERCUTOFF";
	static constexpr const char* FILTER_RESONANCE_PARAM_ID = "FILTERRESONANCE";
	// Filter ADSR
	static constexpr const char* MOD_ATTACK_PARAM_ID = "MODATTACK";
	static constexpr const char* MOD_DECAY_PARAM_ID = "MODDECAY";
	static constexpr const char* MOD_SUSTAIN_PARAM_ID = "MODSUSTAIN";
	static constexpr const char* MOD_RELEASE_PARAM_ID = "MODRELEASE";

	// FX: IDs
	static constexpr const char* FX_TYPE = "FX_TYPE";     // 0=None, 1=Chorus, 2=Flanger, 3=Reverb
	static constexpr const char* FX_BYPASS = "FX_BYPASS";
	static constexpr const char* FX_WET = "FX_WET";
	// Chorus
	static constexpr const char* CH_RATE = "CH_RATE";
	static constexpr const char* CH_DEPTH = "CH_DEPTH";
	static constexpr const char* CH_DELAY_MS = "CH_DELAY_MS";
	static constexpr const char* CH_FEEDBACK = "CH_FEEDBACK";
	// Flanger
	static constexpr const char* FL_RATE = "FL_RATE";
	static constexpr const char* FL_DEPTH = "FL_DEPTH";
	static constexpr const char* FL_DELAY_MS = "FL_DELAY_MS";
	static constexpr const char* FL_FEEDBACK = "FL_FEEDBACK";
	// Reverb
	static constexpr const char* RV_SIZE = "RV_SIZE";
	static constexpr const char* RV_DAMP = "RV_DAMP";
	static constexpr const char* RV_WIDTH = "RV_WIDTH";

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

		// FX
		params.push_back(std::make_unique<juce::AudioParameterChoice>(FX_TYPE, "FX Type", juce::StringArray{ "None", "Chorus", "Flanger", "Reverb" }, 0));

		params.push_back(std::make_unique<juce::AudioParameterBool>(FX_BYPASS, "FX Bypass", false));
		params.push_back(std::make_unique<juce::AudioParameterFloat>(FX_WET, "FX Wet", juce::NormalisableRange<float>(0.0f, 1.0f, 0.0f, 1.0f), 0.3f));

		// Chorus
		params.push_back(std::make_unique<juce::AudioParameterFloat>(CH_RATE, "Chorus Rate (Hz)", juce::NormalisableRange<float>(0.05f, 5.0f, 0.0f, 0.5f), 1.2f));
		params.push_back(std::make_unique<juce::AudioParameterFloat>(CH_DEPTH, "Chorus Depth", juce::NormalisableRange<float>(0.0f, 1.0f, 0.0f, 1.0f), 0.6f));
		params.push_back(std::make_unique<juce::AudioParameterFloat>(CH_DELAY_MS, "Chorus Delay (ms)", juce::NormalisableRange<float>(3.0f, 25.0f, 0.0f, 0.5f), 15.0f));
		params.push_back(std::make_unique<juce::AudioParameterFloat>(CH_FEEDBACK, "Chorus Feedback", juce::NormalisableRange<float>(0.0f, 0.9f, 0.0f, 1.0f), 0.0f));

		// Flanger
		params.push_back(std::make_unique<juce::AudioParameterFloat>(FL_RATE, "Flanger Rate (Hz)", juce::NormalisableRange<float>(0.05f, 2.0f, 0.0f, 0.5f), 0.25f));
		params.push_back(std::make_unique<juce::AudioParameterFloat>(FL_DEPTH, "Flanger Depth", juce::NormalisableRange<float>(0.0f, 1.0f, 0.0f, 1.0f), 0.9f));
		params.push_back(std::make_unique<juce::AudioParameterFloat>(FL_DELAY_MS, "Flanger Delay (ms)", juce::NormalisableRange<float>(0.1f, 5.0f, 0.0f, 0.5f), 1.2f));
		params.push_back(std::make_unique<juce::AudioParameterFloat>(FL_FEEDBACK, "Flanger Feedback", juce::NormalisableRange<float>(0.0f, 0.95f, 0.0f, 1.0f), 0.55f));

		// Reverb
		params.push_back(std::make_unique<juce::AudioParameterFloat>(RV_SIZE, "Reverb Size", juce::NormalisableRange<float>(0.0f, 1.0f), 0.25f));
		params.push_back(std::make_unique<juce::AudioParameterFloat>(RV_DAMP, "Reverb Damping", juce::NormalisableRange<float>(0.0f, 1.0f), 0.4f));
		params.push_back(std::make_unique<juce::AudioParameterFloat>(RV_WIDTH, "Reverb Width", juce::NormalisableRange<float>(0.0f, 1.0f), 1.0f));

		return { params.begin(), params.end() };
	}

};