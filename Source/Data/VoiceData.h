/*
  ==============================================================================

    VoiceData.h
    Created: 4 Dec 2025 5:57:46pm
    Author:  crist

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../SynthVoice.h"

class VoiceData
{
public:
    VoiceData() = default;

    void prepareVoice(SynthVoice& voice, double sampleRate, int samplesPerBlock, int numOutputChannels);
    void applyParams(SynthVoice& voice, const juce::AudioProcessorValueTreeState& apvts, int activeVoices);
};
