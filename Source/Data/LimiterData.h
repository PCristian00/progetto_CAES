/*
  ==============================================================================

    LimiterData.h
    Created: 4 Dec 2025 5:35:38pm
    Author:  crist

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class LimiterData
{
public:
    LimiterData() = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels);
    void reset();
    void process(juce::AudioBuffer<float>& buffer);

    // Parameters
    void setThreshold(float thresholdDb) { limiter.setThreshold(thresholdDb); }
    void setRelease(float releaseMs) { limiter.setRelease(releaseMs); }

private:
    juce::dsp::Limiter<float> limiter;
    bool isPrepared{ false };
};
