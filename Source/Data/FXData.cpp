/*
  ==============================================================================

    FXData.cpp
    Created: 25 Nov 2025 12:49:45pm
    Author:  crist

  ==============================================================================
*/

#include "FXData.h"

/**
 * Prepara la catena DSP degli effetti e il mixer Dry/Wet.
 * - Inizializza Chorus, Flanger e Reverb con parametri di default "sicuri"
 * - Imposta il mixer in modalità totalmente Dry (wet = 0)
 * - Deve essere chiamata prima di qualsiasi elaborazione audio.
 *
 * @param sampleRate        Frequenza di campionamento corrente.
 * @param samplesPerBlock   Dimensione massima del blocco audio.
 * @param numChannels       Numero di canali da processare.
 *
 * Post-condizioni: isPrepared = true
 * Nota: nessuna allocazione dinamica dopo la preparazione (adatto al real-time).
 */
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

/**
 * Resetta lo stato interno degli effetti e del mixer Dry/Wet.
 * Non modifica i parametri impostati in precedenza, solo buffer/stato interno.
 * Usare quando si interrompe il flusso audio o si cambia configurazione.
 */
void FXData::reset()
{
    chain.reset();
    fxMixer.reset();
}

/**
 * Elabora un buffer audio applicando l'effetto selezionato.
 * Logica:
 * 1. Se bypass globale o tipo None: ritorna subito (zero lavoro aggiuntivo).
 * 2. Disattiva tutti gli effetti.
 * 3. Attiva solo l'effetto corrispondente a currentType.
 * 4. Salva copia Dry nel mixer, processa la catena, quindi mix Wet/Dry.
 *
 * @param buffer Buffer audio (in-place).
 * Pre-condizione: prepareToPlay deve essere stato chiamato (isPrepared == true).
 * Tempo reale: nessuna allocazione, solo operazioni su strutture già pronte.
 */
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

/**
 * Aggiorna i parametri del Chorus.
 * Limita i valori in range sicuri per evitare artefatti.
 *
 * @param rateHz       Frequenza di modulazione (0.01..10 Hz).
 * @param depth01      Profondità di modulazione (0..1).
 * @param delayMs      Delay centrale in millisecondi (>= 0.1).
 * @param feedback01   Feedback (0..0.95).
 */
void FXData::setChorus(float rateHz, float depth01, float delayMs, float feedback01)
{
    auto& ch = chain.get<0>();
    ch.setRate(juce::jlimit(0.01f, 10.0f, rateHz));
    ch.setDepth(juce::jlimit(0.0f, 1.0f, depth01));
    ch.setCentreDelay(juce::jmax(0.1f, delayMs));
    ch.setFeedback(juce::jlimit(0.0f, 0.95f, feedback01));
    ch.setMix(1.0f);
}

/**
 * Aggiorna i parametri del Flanger.
 * Range stretti per mantenere carattere tipico e stabilità.
 *
 * @param rateHz       Frequenza LFO (0.01..5 Hz).
 * @param depth01      Profondità modulazione (0..1).
 * @param delayMs      Delay centrale (0.1..5 ms).
 * @param feedback01   Feedback (0..0.95).
 */
void FXData::setFlanger(float rateHz, float depth01, float delayMs, float feedback01)
{
    auto& fl = chain.get<1>();
    fl.setRate(juce::jlimit(0.01f, 5.0f, rateHz));
    fl.setDepth(juce::jlimit(0.0f, 1.0f, depth01));
    fl.setCentreDelay(juce::jlimit(0.1f, 5.0f, delayMs)); // 0.1..5 ms
    fl.setFeedback(juce::jlimit(0.0f, 0.95f, feedback01));
    fl.setMix(1.0f);
}

/**
 * Aggiorna i parametri principali del Reverb (room size, damping, width).
 * Il mix Wet/Dry è gestito esternamente dal DryWetMixer: wetLevel sempre = 1, dryLevel = 0.
 *
 * @param size01     Dimensione stanza (0..1).
 * @param damping01  Damping / attenuazione alte frequenze (0..1).
 * @param width01    Larghezza stereo (0..1).
 */
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
