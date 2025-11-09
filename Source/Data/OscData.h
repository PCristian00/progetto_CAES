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
	/* OscData() : juce::dsp::Oscillator<float>([](float x) { return std::sin(x); })
	 {
	 }*/
	void prepareToPlay(juce::dsp::ProcessSpec& spec);
	void setWaveType(const int choice);
	
	void setWaveFrequency(const int midiNoteNumber);

	// Forse rimuovere?
	void OscData::getNextAudioBlock(juce::dsp::AudioBlock<float>& block);
};