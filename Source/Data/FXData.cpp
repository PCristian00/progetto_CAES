/*
  ==============================================================================

    FXData.cpp
    Created: 25 Nov 2025 12:49:45pm
    Author:  crist

  ==============================================================================
*/

#include "FXData.h"

void FXData::prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.numChannels = static_cast<juce::uint32>(numChannels);

    chain.reset();
    chain.prepare(spec);

    // Default sicuri
    {
        auto& ch = chain.get<0>();
        ch.setRate(0.8f);
        ch.setDepth(0.3f);
        ch.setCentreDelay(8.0f);
        ch.setFeedback(0.0f);
        ch.setMix(1.0f); // usiamo DryWetMixer per il mix globale
    }
    {
        auto& fl = chain.get<1>();
        fl.setRate(0.25f);
        fl.setDepth(0.9f);
        fl.setCentreDelay(1.2f);
        fl.setFeedback(0.55f);
        fl.setMix(1.0f);
    }
    {
        auto& rv = chain.get<2>();
        juce::dsp::Reverb::Parameters p;
        p.roomSize = 0.25f;
        p.damping  = 0.4f;
        p.wetLevel = 1.0f;
        p.dryLevel = 0.0f;
        p.width    = 1.0f;
        p.freezeMode = 0.0f;
        rv.setParameters(p);
    }

    fxMixer.reset();
    fxMixer.prepare(spec);
    fxMixer.setWetMixProportion(0.0f);

    isPrepared = true;
}

void FXData::reset()
{
    chain.reset();
    fxMixer.reset();
}

void FXData::process(juce::AudioBuffer<float>& buffer)
{
    jassert(isPrepared);
    juce::dsp::AudioBlock<float> block(buffer);

    // Bypass globale o tipo None: evita lavoro non necessario
    const bool active = (!bypass) && (currentType != FxType::None);

    // Bypassa tutti
    chain.setBypassed<0>(true);
    chain.setBypassed<1>(true);
    chain.setBypassed<2>(true);

    if (!active)
        return;

    // Attiva solo quello selezionato
    switch (currentType)
    {
        case FxType::Chorus:  chain.setBypassed<0>(false); break;
        case FxType::Flanger: chain.setBypassed<1>(false); break;
        case FxType::Reverb:  chain.setBypassed<2>(false); break;
        default: break;
    }

    fxMixer.pushDrySamples(block);
    chain.process(juce::dsp::ProcessContextReplacing<float>(block));
    fxMixer.mixWetSamples(block);
}

void FXData::setChorus(float rateHz, float depth01, float delayMs, float feedback01)
{
    auto& ch = chain.get<0>();
    ch.setRate(juce::jlimit(0.01f, 10.0f, rateHz));
    ch.setDepth(juce::jlimit(0.0f, 1.0f, depth01));
    ch.setCentreDelay(juce::jmax(0.1f, delayMs));
    ch.setFeedback(juce::jlimit(0.0f, 0.95f, feedback01));
    ch.setMix(1.0f);
}

void FXData::setFlanger(float rateHz, float depth01, float delayMs, float feedback01)
{
    auto& fl = chain.get<1>();
    fl.setRate(juce::jlimit(0.01f, 5.0f, rateHz));
    fl.setDepth(juce::jlimit(0.0f, 1.0f, depth01));
    fl.setCentreDelay(juce::jlimit(0.1f, 5.0f, delayMs)); // 0.1..5 ms
    fl.setFeedback(juce::jlimit(0.0f, 0.95f, feedback01));
    fl.setMix(1.0f);
}

void FXData::setReverb(float size01, float damping01, float width01)
{
    auto& rv = chain.get<2>();
    auto p = rv.getParameters();
    p.roomSize = juce::jlimit(0.0f, 1.0f, size01);
    p.damping  = juce::jlimit(0.0f, 1.0f, damping01);
    p.width    = juce::jlimit(0.0f, 1.0f, width01);
    p.wetLevel = 1.0f; // wet gestito dal mixer
    p.dryLevel = 0.0f;
    rv.setParameters(p);
}
