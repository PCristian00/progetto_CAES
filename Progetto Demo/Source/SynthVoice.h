/*
  ==============================================================================

    SynthVoice.h
    Created: 29 Oct 2017 10:18:29am
    Author:  Joshua Hodge

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthSound.h"

#include <cmath> // std::lround

// Prova più percorsi per trovare il file header di Maximilian. Se nessuno è presente,
// stampa un messaggio di errore a compile-time per guidare la risoluzione.
#if defined(__has_include)
  #if __has_include("maximilian.h")
    #include "maximilian.h"
  #elif __has_include("../Maximilian/maximilian.h")
    #include "../Maximilian/maximilian.h"
  #elif __has_include("../../Maximilian/maximilian.h")
    #include "../../Maximilian/maximilian.h"
  #elif __has_include("Maximilian/maximilian.h")
    #include "Maximilian/maximilian.h"
  #else
    #error "maximilian.h non trovato. Aggiungi la cartella Maximilian a __C/C++ > Additional Include Directories__ o copia il file nella cartella corretta."
  #endif
#else
  #include "../../Maximilian/maximilian.h" // fallback per compiler senza __has_include
#endif

class SynthVoice : public SynthesiserVoice
{
public:
    bool canPlaySound (SynthesiserSound* sound) override
    {
        return dynamic_cast <SynthSound*>(sound) != nullptr;
    }
    
    void setPitchBend(int pitchWheelPos)
    {
        if (pitchWheelPos > 8192)
        {
            // shifting up
            pitchBend = float(pitchWheelPos - 8192) / (16383 - 8192);
        }
        else
        {
            // shifting down
            pitchBend = float(8192 - pitchWheelPos) / -8192;    // negative number
        }
    }

    float pitchBendCents()
    {
        if (pitchBend >= 0.0f)
        {
            // shifting up
            return pitchBend * pitchBendUpSemitones * 100;
        }
        else
        {
            // shifting down
            return pitchBend * pitchBendDownSemitones * 100;
        }
    }

    static double noteHz(int midiNoteNumber, double centsOffset)
    {
        double hertz = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        hertz *= std::pow(2.0, centsOffset / 1200);
        return hertz;
    }

    //=======================================================
    
    // Accetta float* (API esistente) ma converte in int usando rounding per evitare warning di conversione
    void getOscType(float* selection)
    {
        theWave = static_cast<int>(std::lround(*selection));
    }
    
    void getOsc2Type(float* selection)
    {
        theWave2 = static_cast<int>(std::lround(*selection));
    }
    //=======================================================
    
    double setOscType ()
    
    {
        double sample1, sample2;
        
        switch (theWave)
        {
            case 0:
                sample1 = osc1.square(frequency);
                break;
            case 1:
                sample1 = osc1.saw(frequency);
                break;
            default:
                sample1 = osc1.sinewave(frequency);
                break;
        }
        
        switch (theWave2)
        {
            case 0:
                sample2 = osc2.saw(frequency / 2.0);
                break;
            case 1:
                sample2 = osc2.square(frequency / 2.0);
                break;
            default:
                sample2 = osc2.sinewave(frequency / 2.0);
                break;
        }
        
        return sample1 + osc2blend * sample2;
    }
    
    //=======================================================
    
    void getEnvelopeParams(float* attack, float* decay, float* sustain, float* release)
    {
        env1.setAttack(*attack);
        env1.setDecay(*decay);
        env1.setSustain(*sustain);
        env1.setRelease(*release);
    }
    
    //=======================================================
    
    double setEnvelope()
    {
        return env1.adsr(setOscType(), env1.trigger);
    }
    
    //=======================================================
    
    void getWillsParams(float* mGain, float* blend, float* pbup, float* pbdn)
    {
        masterGain = *mGain;
        osc2blend = *blend;
        pitchBendUpSemitones = *pbup;
        pitchBendDownSemitones = *pbdn;
    }
    
    void getFilterParams (float* filterType, float* filterCutoff, float* filterRes)
    {
        filterChoice = static_cast<int>(std::lround(*filterType));
        cutoff = *filterCutoff;
        resonance = *filterRes;
    }
    
    //=======================================================
    
    void startNote (int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) override
    {
        (void)sound; // parametro non usato, evita warning C4100
        noteNumber = midiNoteNumber;
        env1.trigger = 1;
        setPitchBend(currentPitchWheelPosition);
        frequency = noteHz(noteNumber, pitchBendCents());
        level = velocity;
    }
    
    //=======================================================
    
    void stopNote (float velocity, bool allowTailOff) override
    {
        env1.trigger = 0;
        allowTailOff = true;
        
        if (velocity == 0)
            clearCurrentNote();
    }
    
    //=======================================================
    
    void pitchWheelMoved (int newPitchWheelValue) override
    {
        setPitchBend(newPitchWheelValue);
        frequency = noteHz(noteNumber, pitchBendCents());
    }
    
    //=======================================================
    
    void controllerMoved (int controllerNumber, int newControllerValue) override
    {
        (void)controllerNumber;      // parametri non usati, evita warning C4100
        (void)newControllerValue;
    }
    
    //=======================================================
    
    void renderNextBlock (AudioBuffer <float> &outputBuffer, int startSample, int numSamples) override
    {
        for (int sample = 0; sample < numSamples; ++sample)
        {
            for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
            {
                // cast esplicito a float per eliminare warning C4244 (double -> float)
                float out = static_cast<float>(setEnvelope() * masterGain);
                outputBuffer.addSample(channel, startSample, out);
            }
            ++startSample;
        }
    }
    
    //=======================================================
private:
    double level;
    double frequency;
    int theWave = 0;
    int theWave2 = 0;

    float masterGain = 1.0f;
    float osc2blend = 0.0f;

    int noteNumber = 0;
    float pitchBend = 0.0f;
    float pitchBendUpSemitones = 2.0f;
    float pitchBendDownSemitones = 2.0f;

    int filterChoice = 0;
    float cutoff = 0.0f;
    float resonance = 0.0f;
    
    maxiOsc osc1, osc2;
    maxiEnv env1;
};
    