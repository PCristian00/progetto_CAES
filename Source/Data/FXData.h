/*
  ==============================================================================

	FXData.h
	Created: 25 Nov 2025 12:49:45pm
	Author:  crist

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class FXData
{
public:
	enum FxType { None = 0, Chorus = 1, Flanger = 2, Reverb = 3 };

	FXData() = default;

	void prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels);
	void reset();
	void process(juce::AudioBuffer<float>& buffer);

	// Parametri generali
	void setType(FxType type) { currentType = type; }
	void setBypass(bool b) { bypass = b; }
	void setWet(float wet) { fxMixer.setWetMixProportion(juce::jlimit(0.0f, 1.0f, wet)); }

	// Chorus
	void setChorus(float rateHz, float depth01, float delayMs, float feedback01);

	// Flanger (implementato via Chorus con delay corto)
	void setFlanger(float rateHz, float depth01, float delayMs, float feedback01);

	// Reverb
	void setReverb(float size01, float damping01, float width01);

private:
	using Chain = juce::dsp::ProcessorChain<
		juce::dsp::Chorus<float>,  // index 0 = Chorus
		juce::dsp::Chorus<float>,  // index 1 = Flanger-like
		juce::dsp::Reverb          // index 2 = Reverb
	>;

	Chain chain;
	juce::dsp::DryWetMixer<float> fxMixer;

	FxType currentType{ None };
	bool isPrepared{ false };
	bool bypass{ false };
};