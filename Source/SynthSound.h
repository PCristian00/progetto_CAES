/*
  ==============================================================================

	SynthSound.h
	Created: 6 Nov 2025 2:27:44pm
	Author:  crist

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
public:
	SynthSound() {}
	~SynthSound() override {}
	bool appliesToNote(int /*midiNoteNumber*/) override { return true; }
	bool appliesToChannel(int /*midiChannel*/) override { return true; }
};