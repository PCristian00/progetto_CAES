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
};