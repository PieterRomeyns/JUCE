# Tasks: DAW Recording Application

## Status Legend

| Prefix | Meaning |
|---|---|
| `[ ]` | Not started |
| `[~]` | In progress |
| `[x]` | Done — built, tested, and passing |

Work through tasks in order. Do not skip ahead. Mark a task `[x]` only after the build passes and all tests pass.

---

## Phase 1 — Project Skeleton

- [x] **Task 1.1 — CMakeLists.txt for the DAW app**
  Create or extend `CMakeLists.txt` with a new `juce_add_gui_app` target named `MyDAW`. Link `juce_audio_utils`, `juce_audio_processors`, `juce_dsp`, `juce_gui_basics`, `juce_gui_extra`. Set deployment target macOS 11.0, universal binary arm64+x86_64. JUCE 8.0.7 via FetchContent (already cached in `build/_deps`).

- [x] **Task 1.2 — Application entry point**
  Create `Source/Main.cpp` with a `DawApplication : juce::JUCEApplication` subclass. `initialise()` creates a `MainWindow` and shows it. `shutdown()` deletes it. The app should launch and show an empty black window.

- [x] **Task 1.3 — MainWindow shell**
  Create `Source/UI/MainWindow.h/.cpp`. Subclass `juce::DocumentWindow`. Dark background `#1A1A1A`. Resizable, minimum size 1024×600. Contains a placeholder `juce::Component` for now. Build and confirm the window appears.

---

## Phase 2 — Engine Layer (no UI)

- [x] **Task 2.1 — TransportEngine**
  Create `Source/Engine/TransportEngine.h/.cpp`. Implement `play()`, `stop()`, `record()`, `setPosition()`, `getPositionInSamples()`, `getBpm()`, `setBpm()`, `isPlaying()`, `isRecording()`. All state via `std::atomic`. Write a CTest unit test in `tests/TransportEngineTest.cpp` verifying: play sets isPlaying, stop clears it, double-stop resets position to 0, BPM clamp to [20, 300].

- [x] **Task 2.2 — AudioEngine**
  Create `Source/Engine/AudioEngine.h/.cpp`. Subclass `juce::AudioIODeviceCallback`. Own a `juce::AudioDeviceManager`. `initialise()` opens the default device. `audioDeviceIOCallbackWithContext()` is the real-time callback — for now just clear the output buffer. Write a CTest test confirming `initialise()` does not throw.

- [x] **Task 2.3 — MixerEngine**
  Create `Source/Engine/MixerEngine.h/.cpp`. `processBlock()` iterates a `std::vector<std::shared_ptr<TrackProcessor>>` and accumulates into a master buffer. Apply master volume (smoothed with `juce::LinearSmoothedValue`). Write a CTest test: add two tracks outputting constant 0.5, verify master output is 1.0 at master volume 1.0.

- [x] **Task 2.4 — TrackProcessor (audio)**
  Create `Source/Engine/TrackProcessor.h/.cpp` as a `juce::AudioProcessor` subclass. Implement volume/pan with `juce::LinearSmoothedValue`. Implement mute (zero output). Implement solo flag (MixerEngine checks all tracks). Write CTest tests for mute (output = 0) and volume scaling.

- [ ] **Task 2.5 — EffectChain**
  Create `Source/Engine/EffectChain.h/.cpp`. Holds `std::vector<std::unique_ptr<juce::AudioProcessor>>` (max 8). `processBlock()` chains them in series. `setBypass(index, true)` passes audio through unmodified. Write CTest test: chain two gain processors, verify output is input × gain1 × gain2.

---

## Phase 3 — Built-in Effects

- [ ] **Task 3.1 — AmpSimulatorFX**
  Create `Source/Effects/AmpSimulatorFX.h/.cpp`. Subclass `juce::AudioProcessor`. Own a `DigitalAmpDSP` (from `DSP/DSPCoreImpl.h`). APVTS parameters: gain (-24–24 dB), tone (-12–12 dB), drive (0–50), decayTime (0.5–5 s), volume (0–1). `prepareToPlay` → `dsp.initialize()`. `processBlock` → push params then `dsp.process()`.

- [ ] **Task 3.2 — DistortionFX**
  Create `Source/Effects/DistortionFX.h/.cpp`. Hard/soft clip with APVTS parameters: drive (0–100), tone (-12–+12 dB), output (-24–0 dB). Write CTest test: output bounded to [-1, 1] for all inputs.

- [ ] **Task 3.3 — OverdriveFX**
  Create `Source/Effects/OverdriveFX.h/.cpp`. Asymmetric soft-clip. APVTS parameters: drive (0–100), level (0–1), tone (-12–+12 dB). Write CTest test: zero input → zero output; output bounded.

- [ ] **Task 3.4 — PhaserFX**
  Create `Source/Effects/PhaserFX.h/.cpp`. 4-stage all-pass phaser. APVTS parameters: rate (0.1–10 Hz), depth (0–1), feedback (-0.99–0.99). Write CTest test: output is finite; zero input → zero output.

- [ ] **Task 3.5 — ChorusFX**
  Create `Source/Effects/ChorusFX.h/.cpp`. Stereo chorus using a modulated delay line. APVTS parameters: rate (0.1–5 Hz), depth (0–1), delay (5–30 ms), mix (0–1). Write CTest test: at mix=0 output equals input.

- [ ] **Task 3.6 — DelayFX**
  Create `Source/Effects/DelayFX.h/.cpp`. Stereo delay with circular buffer. APVTS parameters: time (1–2000 ms), feedback (0–0.99), mix (0–1). Write CTest test: at feedback=0 and mix=1, output at sample N equals input at sample N-delayInSamples.

---

## Phase 4 — Virtual Instruments

- [ ] **Task 4.1 — SubtractiveSynth**
  Create `Source/Instruments/SubtractiveSynth.h/.cpp`. 8-voice polyphony. Each voice: oscillator (sine/saw/square/triangle), 1-pole LP filter (cutoff 20–20000 Hz, resonance 0–1), ADSR. Responds to MIDI note-on/off. Write CTest test: note-on produces non-zero output.

- [ ] **Task 4.2 — DrumMachine**
  Create `Source/Instruments/DrumMachine.h/.cpp`. 16 pads, each plays a WAV sample. 16-step sequencer per pad. Responds to MIDI note-on for live playing. Write CTest test: triggering pad 0 produces non-zero output.

---

## Phase 5 — Model Layer

- [ ] **Task 5.1 — Clip model**
  Create `Source/Model/Clip.h/.cpp`. Fields: `startBeat`, `durationBeats`, `audioFile`, `midiSequence`, `type`. Serialise/deserialise to `juce::ValueTree`.

- [ ] **Task 5.2 — Track model**
  Create `Source/Model/Track.h/.cpp`. Fields: name, type, volume, pan, muted, soloed, clips vector. Serialise/deserialise to `juce::ValueTree`.

- [ ] **Task 5.3 — Project model and serialiser**
  Create `Source/Model/Project.h/.cpp` and `Source/Model/ProjectSerializer.h/.cpp`. Serialiser writes/reads `.dawproject` XML via `juce::ValueTree`. Write CTest test: create project, add track with clip, serialise, deserialise, verify all fields match.

---

## Phase 6 — MIDI and Plugin Host

- [ ] **Task 6.1 — MidiEngine**
  Create `Source/Engine/MidiEngine.h/.cpp`. Subclass `juce::MidiInputCallback`. Route incoming MIDI to armed tracks. Record MIDI events into a clip during recording. Write CTest test: send note-on, verify it appears in the recorded clip.

- [ ] **Task 6.2 — PluginHost**
  Create `Source/Engine/PluginHost.h/.cpp`. Use `juce::AudioPluginFormatManager` with VST3 and AU formats. `loadPlugin()` returns `nullptr` on failure without crashing. Write CTest test: `loadPlugin()` with an invalid description returns nullptr and does not throw.

---

## Phase 7 — Wire Engine to Audio Callback

- [ ] **Task 7.1 — Connect MixerEngine to AudioEngine callback**
  In `AudioEngine::audioDeviceIOCallbackWithContext()`, call `mixerEngine.processBlock()`. Verify silence is produced when no tracks exist.

- [ ] **Task 7.2 — Connect TrackProcessor to audio file playback**
  In `TrackProcessor::processBlock()`, read samples from the clip's audio file at the current transport position using `juce::AudioFormatReader`. Handle end-of-file gracefully.

- [ ] **Task 7.3 — Connect MIDI tracks to instruments**
  In `TrackProcessor::processBlock()` for MIDI tracks, pass the MIDI buffer to the assigned `VirtualInstrument::processBlock()` and mix its audio output into the track buffer.

---

## Phase 8 — UI

- [ ] **Task 8.1 — TransportBar**
  Create `Source/UI/TransportBar.h/.cpp`. Play, Stop, Record buttons. BPM field. Position display. Keyboard shortcuts: Space = play/stop, R = record.

- [ ] **Task 8.2 — ArrangementView**
  Create `Source/UI/ArrangementView.h/.cpp`. Scrollable timeline. Ruler at top. One lane per track. Playhead line, timer-driven repaint at 30 fps.

- [ ] **Task 8.3 — TrackHeader**
  Create `Source/UI/TrackHeader.h/.cpp`. Track name, mute (M), solo (S), record-arm (R), mini volume fader. Width: 180 px.

- [ ] **Task 8.4 — ClipComponent**
  Create `Source/UI/ClipComponent.h/.cpp`. Coloured rectangle. Audio clips: waveform via `juce::AudioThumbnail`. MIDI clips: miniature note bars. Draggable. Right-click: Delete, Duplicate, Split at Playhead.

- [ ] **Task 8.5 — MixerView**
  Create `Source/UI/MixerView.h/.cpp`. Horizontal row of `ChannelStrip` components plus a Master strip.

- [ ] **Task 8.6 — ChannelStrip**
  Create `Source/UI/ChannelStrip.h/.cpp`. Track name, 8 insert slots, pan knob, stereo level meters, volume fader, mute/solo buttons. Width: 80 px.

- [ ] **Task 8.7 — EffectRackView and EffectEditorView**
  Create `Source/UI/EffectRackView.h/.cpp`. Shows insert chain for selected track. Clicking a slot opens `EffectEditorView` using `juce::GenericAudioProcessorEditor`.

- [ ] **Task 8.8 — PianoRollEditor**
  Create `Source/UI/PianoRollEditor.h/.cpp`. Opens on MIDI clip double-click. Piano keyboard left. Notes as rectangles. Click to add, drag to move/resize, right-click to delete. Velocity bars at bottom.

- [ ] **Task 8.9 — DrumPadGrid**
  Create `Source/UI/DrumPadGrid.h/.cpp`. 4×4 pad grid. 16-step sequencer grid below each pad.

- [ ] **Task 8.10 — AudioSettingsDialog**
  Create `Source/UI/AudioSettingsDialog.h/.cpp`. Modal dialog wrapping `juce::AudioDeviceSelectorComponent`. Accessible via Cmd+,.

---

## Phase 9 — Integration and Polish

- [ ] **Task 9.1 — Layout integration**
  Wire all UI into `MainWindow`. Top: `TransportBar`. Left: `TrackHeader` column. Centre: `ArrangementView`. Bottom: `MixerView`. Menu bar with File, Edit, Settings.

- [ ] **Task 9.2 — Undo/Redo**
  Implement `juce::UndoManager` for track add/delete, clip move/delete, parameter changes. Cmd+Z / Cmd+Shift+Z.

- [ ] **Task 9.3 — Audio recording to file**
  Write captured buffer to WAV in `<projectFolder>/audio/` using `juce::WavAudioFormat`. Update clip path and trigger waveform thumbnail generation.

- [ ] **Task 9.4 — Level metering**
  Feed peak levels from `TrackProcessor` to `ChannelStrip` meters via `juce::AbstractFifo`. Clip indicator lights red above 0 dBFS, resets on click.

- [ ] **Task 9.5 — Property-based tests**
  Write CTest tests verifying all five correctness properties from Requirement 12.

- [ ] **Task 9.6 — Final build and AU validation**
  Build Release universal binary. Run `auval -v aufx Amp1 MPlg`. Run `ctest --output-on-failure`. Fix any failures.
