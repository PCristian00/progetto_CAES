/*
  ==============================================================================

	OscData.h
	Created: 9 Nov 2025 4:43:23pm
	Author:  crist

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class OscData :public juce::dsp::Oscillator<float>
{
public:
	void prepare(juce::dsp::ProcessSpec& spec);
	void setWaveType(const int choice);

	void setFrequency(const int midiNoteNumber);

	// Forse rimuovere?
	// void getNextAudioBlock(juce::dsp::AudioBlock<float>& block);

	float processSample(float input);

	void setFmParams(const float depth, const float freq);

private:
	// juce::dsp::Oscillator<float> fmOsc{ [](float x) { return std::sin(x); } };
	float fmMod{ 0.0f };
	float fmDepth{ 0.0f };
	int lastMidiNote{ 0 };
};