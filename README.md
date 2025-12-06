# SubSynth

## Introduzione

"SubSynth" è un plugin audio JUCE, progettato come sintetizzatore polifonico con effetti post‑processing e gestione completa dei preset.

### Cosa fa
- Genera suoni sintetizzati tramite un oscillatore con forme d’onda selezionabili (Sine, Saw, Square, Triangle).
- Supporta polifonia dinamica (fino a 48 voci), con scaling automatico del gain per evitare clipping in condizioni polifoniche.
- Applica inviluppi ADSR all’ampiezza e un secondo ADSR per modulare i parametri del filtro.
- Filtra il segnale per‑voce con un filtro State Variable (Low‑Pass, Band‑Pass, High‑Pass), con cutoff modulato dall’inviluppo di modulazione.
- Processa effetti post‑synth (Chorus, Flanger, Reverb) con controllo Dry/Wet e bypass.
- Protegge l’uscita con un Limiter finale (threshold in dB e release in ms).
- Gestisce parametri e stato tramite AudioProcessorValueTreeState (APVTS), con salvataggio/caricamento in XML (inclusa la versione e il nome preset).

### Architettura in breve
- **Core DSP**: `SynthVoice` (oscillatore, ADSR amp, ADSR mod, filtro, gain) gestito da `juce::Synthesiser` nel `SubSynthAudioProcessor`.
- **Effetti**: classe `FXData` incapsula una `juce::dsp::ProcessorChain` (Chorus, Flanger, Reverb) e un `DryWetMixer`. Un Limiter viene applicato alla fine della catena.
- **Parametri**: definiti in `Parameters.h` (oscillatore, voci, FM, ADSR, filtro, FX, limiter) e letti/aggiornati ad ogni blocco in `processBlock`.
- **Stato/Preset**: `APVTS` serializza in XML e ripristina mantenendo proprietà obbligatorie (`version`, `presetName`).
- **UI**: componenti dedicati (`OscComponent`, `FXComponent`, `LimiterComponent`, `PresetPanel`, utility in `Utils`) collegano i controlli ai parametri APVTS.

### Flusso di esecuzione
1. **Avvio**: il processor inizializza APVTS, preset manager e imposta il numero di voci iniziale.
2. **prepareToPlay**: prepara voci, effetti e limiter con il `ProcessSpec` corretto.
3. **processBlock**:
   - Aggiorna il numero di voci richiesto in tempo reale.
   - Applica i parametri di voce (inviluppi, filtro, gain). Renderizza il synth (dry).
   - Aggiorna e applica FX (tipo, bypass, wet, parametri specifici).
   - Applica il Limiter all’uscita.
4. **Stato**: salvataggio/caricamento XML con proprietà obbligatorie.

### Interfaccia e controlli principali
- **Oscillatore**: selezione forma d’onda, `FM Frequency`, `FM Depth`.
- **Voci**: `Number of Voices`.
- **ADSR Amp**: `Attack`, `Decay`, `Sustain`, `Release`, `Gain`.
- **Filtro**: `Filter Type`, `Filter Cutoff`, `Filter Resonance`. ADSR Mod: `Mod Attack`, `Mod Decay`, `Mod Sustain`, `Mod Release`.
- **FX**: `FX Type`, `FX Bypass`, `FX Wet`, parametri di Chorus/Flanger/Reverb.
- **Limiter**: `Limiter Threshold (dB)`, `Limiter Release (ms)`.

### Catena del segnale
Synth (per‑voce) → Somma → FX → Limiter → Uscita.

## Installazione

Per installare il plugin SubSynth, segui questi passaggi:
1. Clona il repository: `git clone <repository-url>`.
2. Apri il progetto in un IDE compatibile con JUCE (ad esempio, Projucer).
3. Compila il progetto per la tua piattaforma (Windows, macOS, Linux).
4. Installa il plugin nel tuo ambiente di lavoro audio preferito.
