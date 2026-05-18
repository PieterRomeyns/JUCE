# Requirements: DAW Recording Application

## Introduction

A full digital audio workstation (DAW) built as a native macOS desktop application using JUCE 8 and C++17. The application supports multi-track audio and MIDI recording, a built-in effects chain (amp simulation, distortion, overdrive, phaser, chorus, delay), virtual instruments, drum tracks, and a dark Cubase-style UI. The amp plugin DSP chain already exists in `DSP/` and `MyAmpPlugin.cpp/.h` and must be integrated as a first-class built-in effect rather than rebuilt.

---

## Glossary

| Term | Definition |
|---|---|
| Track | A single lane in the timeline that holds audio clips or MIDI clips |
| Clip | A region on a track containing recorded audio or MIDI data |
| Transport | Play, stop, record, rewind controls |
| Insert | An effect slot on a track's signal chain |
| Send | A parallel routing path from a track to an effects bus |
| Plugin Host | The subsystem that loads and runs AU/VST3 third-party plugins |
| Arrangement View | The main timeline showing all tracks and clips |
| Mixer View | The vertical channel strip view for level/pan/routing |
| APVTS | JUCE AudioProcessorValueTreeState — parameter management |

---

## Requirement 1: Application Shell

**User Story:** As a user, I want to launch the app and see a working DAW window, so that I can start a project immediately.

### Acceptance Criteria

1. WHEN the application launches, THEN a main window SHALL appear with a dark background (≤ #1E1E1E) and a toolbar containing Transport controls.
2. WHEN the application launches, THEN the window SHALL contain three dockable panels: Arrangement View (top), Mixer View (bottom), and a Browser/Library panel (left).
3. WHEN the user resizes the window, THEN all panels SHALL resize proportionally without layout breakage.
4. WHEN the application launches, THEN a default empty project SHALL be created with 0 tracks and the transport at position 0.
5. THE application SHALL be a JUCE `JUCEApplication` subclass — no Objective-C, no AppKit, no Cocoa directly.

---

## Requirement 2: Transport

**User Story:** As a user, I want play, stop, record, and rewind controls, so that I can control playback and recording.

### Acceptance Criteria

1. WHEN the user clicks Play, THEN the transport SHALL start advancing the playhead at the current BPM.
2. WHEN the user clicks Stop, THEN the transport SHALL stop and the playhead SHALL remain at its current position.
3. WHEN the user clicks Stop a second time while already stopped, THEN the playhead SHALL return to position 0.
4. WHEN the user clicks Record while the transport is stopped, THEN the transport SHALL start and all record-armed tracks SHALL begin capturing audio or MIDI.
5. WHEN the user presses Space, THEN play/stop SHALL toggle.
6. WHEN the user presses R, THEN record SHALL arm/disarm.
7. THE transport SHALL display current position as bars:beats:ticks and as minutes:seconds.
8. THE BPM SHALL be editable via a numeric field in the toolbar, range 20–300 BPM.
9. THE time signature SHALL be editable (numerator 1–16, denominator 1, 2, 4, 8, 16).

---

## Requirement 3: Track Management

**User Story:** As a user, I want to create, delete, and configure up to 16 tracks, so that I can organise my recording session.

### Acceptance Criteria

1. THE application SHALL support up to 16 tracks simultaneously.
2. WHEN the user adds a track, THEN they SHALL be prompted to choose: Audio Track, MIDI Track, or Drum Track.
3. WHEN an Audio Track is created, THEN it SHALL have: name, volume fader (0–1), pan knob (-1 to +1), mute button, solo button, record-arm button, and an effects insert chain.
4. WHEN a MIDI Track is created, THEN it SHALL have the same controls as an Audio Track plus a virtual instrument selector.
5. WHEN a Drum Track is created, THEN it SHALL be a MIDI track pre-configured with a built-in drum machine instrument.
6. WHEN the user renames a track, THEN the new name SHALL persist across save/load.
7. WHEN solo is active on one or more tracks, THEN all non-soloed tracks SHALL be silenced.
8. WHEN a track is muted, THEN its audio output SHALL be silenced without removing it from the mix.
9. WHEN the user deletes a track, THEN all its clips SHALL be removed and the track lane SHALL disappear from the Arrangement View.

---

## Requirement 4: Audio Recording

**User Story:** As a user, I want to record audio from a microphone or instrument input, so that I can capture performances.

### Acceptance Criteria

1. WHEN the user arms an audio track and starts recording, THEN audio from the selected input device SHALL be captured to a new clip on that track.
2. WHEN recording stops, THEN the captured audio SHALL be written to a WAV file in the project folder and the clip SHALL display a waveform.
3. THE application SHALL support sample rates of 44100, 48000, 88200, and 96000 Hz.
4. THE application SHALL support buffer sizes of 64, 128, 256, 512, 1024, and 2048 samples.
5. WHEN the buffer size changes during a session, THEN all DSP stages SHALL be re-prepared without audio dropout on the next block.
6. THE application SHALL use JUCE's `AudioDeviceManager` for device selection and configuration.
7. WHEN no audio input device is available, THEN the application SHALL display a warning and disable record-arm buttons.

---

## Requirement 5: MIDI Recording

**User Story:** As a user, I want to record MIDI from a keyboard or controller, so that I can capture note performances and edit them.

### Acceptance Criteria

1. WHEN a MIDI track is armed and recording is active, THEN incoming MIDI events SHALL be captured to a MIDI clip.
2. WHEN recording stops, THEN the MIDI clip SHALL be stored in memory and displayed as a piano-roll bar in the Arrangement View.
3. THE application SHALL support MIDI input from any connected MIDI device via JUCE's `MidiInput`.
4. WHEN the user double-clicks a MIDI clip, THEN a piano-roll editor SHALL open showing notes as coloured bars.
5. IN the piano-roll editor, the user SHALL be able to add, move, resize, and delete notes.
6. MIDI velocity SHALL be editable per note in the piano-roll editor.

---

## Requirement 6: Virtual Instruments

**User Story:** As a user, I want to assign virtual instruments to MIDI tracks, so that I can hear and record synthesised sounds.

### Acceptance Criteria

1. THE application SHALL include at least one built-in synthesiser instrument: a simple subtractive synth with oscillator (sine, saw, square, triangle), filter (low-pass with cutoff and resonance), and ADSR envelope.
2. THE application SHALL include a built-in drum machine instrument with 16 pads, each assignable to a sample from the sample library.
3. WHEN a MIDI track has a virtual instrument assigned, THEN MIDI events from that track SHALL be routed to the instrument and its audio output SHALL be mixed into the master bus.
4. THE application SHALL be able to host third-party AU and VST3 instruments via JUCE's `AudioPluginHost` infrastructure.
5. WHEN a third-party instrument plugin fails to load, THEN the track SHALL fall back to silence and display an error badge — the application SHALL NOT crash.

---

## Requirement 7: Built-in Effects

**User Story:** As a user, I want a set of built-in effects I can insert on any track, so that I can shape the sound without needing third-party plugins.

### Acceptance Criteria

1. THE following built-in effects SHALL be available as inserts on any track:
   - **Amp Simulator** — the existing `DigitalAmpDSP` chain (Gain, Tone, Drive, Decay Time, Volume)
   - **Distortion** — hard/soft clip with drive and tone controls
   - **Overdrive** — asymmetric soft-clip modelled on tube overdrive (drive, level, tone)
   - **Phaser** — 4-stage all-pass phaser with rate, depth, and feedback
   - **Chorus** — stereo chorus with rate, depth, delay, and mix
   - **Delay** — stereo delay with time (ms or sync-to-BPM), feedback, and wet/dry mix
2. EACH built-in effect SHALL be implemented as a `juce::AudioProcessor` subclass with APVTS parameters.
3. WHEN an effect is added to a track's insert chain, THEN it SHALL process audio in series after the previous insert.
4. WHEN an effect is bypassed, THEN the audio SHALL pass through unmodified with zero additional latency.
5. EACH effect's parameters SHALL be automatable via the MIDI CC automation system.
6. THE amp simulator SHALL reuse `DigitalAmpDSP` from `DSP/DSPCoreImpl.h` — it SHALL NOT be reimplemented.

---

## Requirement 8: Mixer

**User Story:** As a user, I want a mixer view with channel strips, so that I can balance levels and route audio.

### Acceptance Criteria

1. THE Mixer View SHALL display one channel strip per track plus a Master channel strip.
2. EACH channel strip SHALL contain: track name label, volume fader (vertical, 0 dB default), pan knob, mute button, solo button, peak level meter (stereo, -60 dB to 0 dBFS), and insert effect slots (up to 8).
3. WHEN the user drags a volume fader, THEN the track's output level SHALL change in real time with no audible zipper noise (parameter smoothing SHALL be applied).
4. THE Master channel strip SHALL control the final output level sent to the audio device.
5. WHEN a track's peak level exceeds 0 dBFS, THEN the level meter clip indicator SHALL light red and remain lit until the user clicks it.

---

## Requirement 9: Arrangement View

**User Story:** As a user, I want a timeline view showing all tracks and clips, so that I can arrange my session.

### Acceptance Criteria

1. THE Arrangement View SHALL display a horizontal timeline with a ruler showing bars and beats.
2. EACH track SHALL occupy a horizontal lane in the Arrangement View.
3. WHEN a clip exists on a track, THEN it SHALL be displayed as a coloured rectangle in the correct time position.
4. Audio clips SHALL display a waveform preview inside the rectangle.
5. MIDI clips SHALL display a miniature piano-roll preview inside the rectangle.
6. WHEN the user clicks and drags a clip, THEN it SHALL move to the new position on the timeline.
7. WHEN the user right-clicks a clip, THEN a context menu SHALL offer: Delete, Duplicate, Split at Playhead.
8. THE playhead SHALL be a vertical line that advances during playback and can be dragged to any position.
9. WHEN the user scrolls horizontally, THEN the timeline SHALL scroll. WHEN the user pinches or uses Ctrl+scroll, THEN the timeline SHALL zoom in/out.

---

## Requirement 10: Project Save and Load

**User Story:** As a user, I want to save and load projects, so that I can continue work across sessions.

### Acceptance Criteria

1. WHEN the user saves a project (Cmd+S), THEN a `.dawproject` file SHALL be written containing: BPM, time signature, all track metadata, all clip positions and durations, all effect parameters, and paths to audio files.
2. WHEN the user opens a project (Cmd+O), THEN the application SHALL restore the session exactly as saved.
3. WHEN an audio file referenced by a project cannot be found, THEN the clip SHALL display an "offline" badge and the application SHALL NOT crash.
4. THE project file format SHALL be XML-based (JUCE `ValueTree` serialised to XML).
5. WHEN the project has unsaved changes and the user tries to close or open another project, THEN a save confirmation dialog SHALL appear.

---

## Requirement 11: Audio Device Settings

**User Story:** As a user, I want to configure my audio interface, so that I can use my preferred hardware.

### Acceptance Criteria

1. THE application SHALL provide an Audio Settings dialog (accessible from the menu) showing: input device, output device, sample rate, and buffer size.
2. WHEN the user changes the audio device or sample rate, THEN the engine SHALL reinitialise without crashing.
3. THE application SHALL use JUCE's built-in `AudioDeviceSelectorComponent` for the settings UI.

---

## Requirement 12: Correctness Properties (Property-Based Testing)

**User Story:** As a developer, I want formal correctness properties verified by tests, so that the DSP and engine behave correctly under all valid inputs.

### Acceptance Criteria

1. **Latency invariant:** FOR ALL valid buffer sizes B ∈ {64, 128, 256, 512, 1024, 2048}, the measured processing time of `DigitalAmpDSP::process()` SHALL be less than one buffer period (B / sampleRate × 1000 ms).
2. **Output bounds invariant:** FOR ALL inputs x ∈ [-1, 1] and all parameter combinations, the output of the full DSP chain SHALL remain in [-1, 1].
3. **State round-trip invariant:** FOR ANY project state S, serialise(S) followed by deserialise() SHALL produce a state S' where all parameter values are equal to S within float epsilon.
4. **Silence invariant:** WHEN all track volumes are 0, the master output SHALL be exactly 0.0 for all samples.
5. **Mute invariant:** WHEN a track is muted, its contribution to the master mix SHALL be exactly 0.0.
