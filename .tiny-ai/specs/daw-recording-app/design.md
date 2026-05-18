# Design: DAW Recording Application

## Overview

A native macOS DAW built entirely in JUCE 8 / C++17. The architecture follows a layered model: an audio engine at the bottom, a project/state model in the middle, and a JUCE Component-based UI on top. The existing `DigitalAmpDSP` chain is reused as a first-class built-in effect. No Objective-C, no AppKit — everything goes through JUCE.

---

## Architecture Diagram

```
┌─────────────────────────────────────────────────────────┐
│                        UI Layer                         │
│  MainWindow  ArrangementView  MixerView  PianoRollEditor│
│  TransportBar  TrackHeader  ChannelStrip  EffectRack    │
└────────────────────────┬────────────────────────────────┘
                         │ JUCE Component hierarchy
┌────────────────────────▼────────────────────────────────┐
│                    Project Model                        │
│  Project  Track  Clip  EffectChain  AutomationLane      │
│  ProjectSerializer (ValueTree XML)                      │
└────────────────────────┬────────────────────────────────┘
                         │ owns / drives
┌────────────────────────▼────────────────────────────────┐
│                    Engine Layer                         │
│  AudioEngine  TransportEngine  MixerEngine              │
│  TrackProcessor  PluginHost  MidiEngine                 │
└────────────────────────┬────────────────────────────────┘
                         │ JUCE audio callbacks
┌────────────────────────▼────────────────────────────────┐
│                    DSP Layer                            │
│  DigitalAmpDSP  DistortionFX  OverdriveFX  PhaserFX    │
│  ChorusFX  DelayFX  SubtractiveSynth  DrumMachine       │
└─────────────────────────────────────────────────────────┘
```

---

## Component Breakdown

### 1. Application Entry Point

```cpp
class DawApplication : public juce::JUCEApplication
{
    void initialise (const juce::String&) override;
    void shutdown()  override;
    const juce::String getApplicationName() override { return "MyDAW"; }
};
```

`MainWindow` is created in `initialise()`. It owns the `AudioEngine` and `Project`.

---

### 2. Audio Engine

```cpp
class AudioEngine : public juce::AudioIODeviceCallback
{
public:
    void initialise();
    void audioDeviceIOCallbackWithContext(...) override; // real-time callback
    void audioDeviceAboutToStart(juce::AudioIODevice*) override;
    void audioDeviceStopped() override;

    TransportEngine&  transport();
    MixerEngine&      mixer();
    MidiEngine&       midi();
    juce::AudioDeviceManager& deviceManager();
};
```

`AudioEngine` owns the `juce::AudioDeviceManager`. The real-time callback calls `MixerEngine::processBlock()` which iterates all `TrackProcessor` instances.

---

### 3. Transport Engine

```cpp
class TransportEngine
{
public:
    void play();
    void stop();
    void record();
    void setPosition (double barsBeats);
    double getPositionInSamples() const noexcept;
    double getBpm()  const noexcept;
    void   setBpm (double bpm);
    bool   isPlaying()   const noexcept;
    bool   isRecording() const noexcept;
};
```

Maintains a sample-accurate position counter. Thread-safe via `std::atomic`.

---

### 4. Mixer Engine

```cpp
class MixerEngine
{
public:
    void prepareToPlay (double sampleRate, int blockSize);
    void processBlock  (juce::AudioBuffer<float>& masterBus,
                        juce::MidiBuffer& midi);
    void addTrack    (std::shared_ptr<TrackProcessor>);
    void removeTrack (int index);
};
```

Iterates `TrackProcessor` list, accumulates into `masterBus`. Applies master volume.

---

### 5. Track Processor

```cpp
class TrackProcessor : public juce::AudioProcessor
{
public:
    enum class Type { Audio, Midi, Drum };

    void prepareToPlay (double sr, int blockSize) override;
    void processBlock  (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    EffectChain& effectChain();
    void setVolume (float v);   // smoothed
    void setPan    (float p);   // smoothed
    void setMuted  (bool m);
    void setSoloed (bool s);
    void armForRecord (bool arm);
};
```

For Audio tracks: reads from the clip at the current transport position, applies the effect chain, applies volume/pan.
For MIDI tracks: routes MIDI to the assigned `VirtualInstrument`, then applies the effect chain.

---

### 6. Effect Chain

```cpp
class EffectChain
{
public:
    void addEffect    (std::unique_ptr<juce::AudioProcessor> fx);
    void removeEffect (int index);
    void moveEffect   (int from, int to);
    void setBypass    (int index, bool bypassed);
    void prepareToPlay (double sr, int blockSize);
    void processBlock  (juce::AudioBuffer<float>&, juce::MidiBuffer&);
};
```

Holds up to 8 inserts. Each insert is a `juce::AudioProcessor`. Built-in effects are subclasses of `juce::AudioProcessor` with APVTS.

---

### 7. Built-in Effects

Each effect is a self-contained `juce::AudioProcessor` subclass:

| Class | Parameters |
|---|---|
| `AmpSimulatorFX` | gain, tone, drive, decayTime, volume — wraps `DigitalAmpDSP` |
| `DistortionFX` | drive (0–100), tone (-12–+12 dB), output (-24–0 dB) |
| `OverdriveFX` | drive (0–100), level (0–1), tone (-12–+12 dB) |
| `PhaserFX` | rate (0.1–10 Hz), depth (0–1), feedback (-1–+1), stages (2/4/8) |
| `ChorusFX` | rate (0.1–5 Hz), depth (0–1), delay (5–30 ms), mix (0–1) |
| `DelayFX` | time (1–2000 ms or sync), feedback (0–0.99), mix (0–1), stereoSpread |

All parameters registered via APVTS. All processing is real-time safe.

**`AmpSimulatorFX` implementation note:**
```cpp
class AmpSimulatorFX : public juce::AudioProcessor
{
    DigitalAmpDSP dsp; // from DSP/DSPCoreImpl.h — NOT reimplemented
    juce::AudioProcessorValueTreeState apvts;
    // prepareToPlay delegates to dsp.initialize()
    // processBlock delegates to dsp.process()
};
```

---

### 8. Virtual Instruments

```cpp
class VirtualInstrument : public juce::AudioProcessor
{
public:
    virtual void handleMidiMessage (const juce::MidiMessage&) = 0;
};

class SubtractiveSynth : public VirtualInstrument { ... };
class DrumMachine      : public VirtualInstrument { ... };
```

`SubtractiveSynth`: polyphonic (8 voices), oscillator (sine/saw/square/triangle), 1-pole LP filter with cutoff + resonance, ADSR.

`DrumMachine`: 16 pads, each plays a sample from the `SampleLibrary`. Step sequencer with 16 steps per pad, 1–4 bars.

---

### 9. Plugin Host (Third-party AU/VST3)

```cpp
class PluginHost
{
public:
    void scanForPlugins();
    std::unique_ptr<juce::AudioPluginInstance>
        loadPlugin (const juce::PluginDescription&, juce::String& error);
    const juce::KnownPluginList& getKnownPlugins() const;
};
```

Uses `juce::AudioPluginFormatManager` with `juce::VST3PluginFormat` and `juce::AudioUnitPluginFormat`. Plugin scan runs on a background thread. Failed loads return `nullptr` — the track falls back to silence.

---

### 10. MIDI Engine

```cpp
class MidiEngine : public juce::MidiInputCallback
{
public:
    void handleIncomingMidiMessage (juce::MidiInput*, const juce::MidiMessage&) override;
    void routeToTrack (int trackIndex, const juce::MidiMessage&);
    void startRecordingToClip (MidiClip&);
    void stopRecording();
};
```

Receives MIDI on the message thread, timestamps events, routes to armed tracks.

---

### 11. Project Model

```cpp
struct Clip {
    double startBeat;
    double durationBeats;
    juce::File audioFile;
    juce::MidiMessageSequence midi;
};

struct Track {
    juce::String name;
    TrackProcessor::Type type;
    float volume = 1.0f, pan = 0.0f;
    bool muted = false, soloed = false;
    std::vector<Clip> clips;
};

struct Project {
    double bpm = 120.0;
    int timeSigNumerator = 4, timeSigDenominator = 4;
    std::vector<Track> tracks;
    juce::File projectFile;
};
```

---

### 12. Project Serialisation

Uses `juce::ValueTree` serialised to XML (`.dawproject` extension).

```xml
<Project bpm="120" timeSigNum="4" timeSigDen="4">
  <Track name="Guitar" type="audio" volume="0.8" pan="0.0">
    <EffectChain>
      <Effect type="AmpSimulator" gain="0" tone="0" drive="10"/>
    </EffectChain>
    <Clip start="0" duration="4" file="audio/take1.wav"/>
  </Track>
</Project>
```

---

### 13. UI Components

All UI is JUCE `Component` subclasses. Dark theme: background `#1A1A1A`, panel borders `#2D2D2D`, accent `#E8A020` (amber, Cubase-inspired).

| Component | Responsibility |
|---|---|
| `MainWindow` | Top-level `juce::DocumentWindow`, owns layout |
| `TransportBar` | BPM field, position display, play/stop/record buttons |
| `ArrangementView` | Scrollable timeline, track lanes, clip rendering |
| `TrackHeader` | Name, mute, solo, arm, volume mini-fader per lane |
| `ClipComponent` | Waveform or MIDI preview rectangle |
| `MixerView` | Horizontal row of `ChannelStrip` components |
| `ChannelStrip` | Fader, pan, meters, insert slots |
| `EffectRackView` | List of inserts for a selected track |
| `EffectEditorView` | Generic parameter editor for a selected effect |
| `PianoRollEditor` | Note editor for MIDI clips |
| `DrumPadGrid` | 16-pad grid for the drum machine |
| `AudioSettingsDialog` | Wraps `juce::AudioDeviceSelectorComponent` |

---

### 14. Threading Model

| Thread | Responsibility |
|---|---|
| Audio thread | `AudioEngine::audioDeviceIOCallbackWithContext` — real-time, no allocations, no locks |
| Message thread | UI events, MIDI input callbacks, plugin scanning |
| Background thread | Audio file reading/writing, plugin scan, project save/load |

Communication between threads uses `juce::AbstractFifo` for audio→UI metering, and `std::atomic` for transport state.

---

### 15. File Layout

```
MyDAW/
├── Source/
│   ├── Main.cpp
│   ├── Engine/
│   │   ├── AudioEngine.h/.cpp
│   │   ├── TransportEngine.h/.cpp
│   │   ├── MixerEngine.h/.cpp
│   │   ├── TrackProcessor.h/.cpp
│   │   ├── EffectChain.h/.cpp
│   │   ├── MidiEngine.h/.cpp
│   │   └── PluginHost.h/.cpp
│   ├── Effects/
│   │   ├── AmpSimulatorFX.h/.cpp
│   │   ├── DistortionFX.h/.cpp
│   │   ├── OverdriveFX.h/.cpp
│   │   ├── PhaserFX.h/.cpp
│   │   ├── ChorusFX.h/.cpp
│   │   └── DelayFX.h/.cpp
│   ├── Instruments/
│   │   ├── SubtractiveSynth.h/.cpp
│   │   └── DrumMachine.h/.cpp
│   ├── Model/
│   │   ├── Project.h/.cpp
│   │   ├── Track.h/.cpp
│   │   ├── Clip.h/.cpp
│   │   └── ProjectSerializer.h/.cpp
│   └── UI/
│       ├── MainWindow.h/.cpp
│       ├── TransportBar.h/.cpp
│       ├── ArrangementView.h/.cpp
│       ├── TrackHeader.h/.cpp
│       ├── ClipComponent.h/.cpp
│       ├── MixerView.h/.cpp
│       ├── ChannelStrip.h/.cpp
│       ├── EffectRackView.h/.cpp
│       ├── PianoRollEditor.h/.cpp
│       ├── DrumPadGrid.h/.cpp
│       └── AudioSettingsDialog.h/.cpp
├── DSP/                          ← existing, do not modify
└── tests/
```

---

### 16. Error Handling

| Failure | Response |
|---|---|
| Audio device unavailable | Show dialog, disable record-arm, continue in silence |
| Plugin load failure | Log error, track outputs silence, show error badge on insert slot |
| Project file corrupt/missing audio | Load what is available, show offline badges on affected clips |
| DSP overload (CPU > 90%) | `DigitalAmpDSP` complexity reduction activates automatically |
| Buffer size change mid-session | All `prepareToPlay` called on audio thread before next block |
