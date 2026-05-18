# Rules for DAW Recording App Implementation

## Conduct

- Work through tasks in the order defined in `tasks.md`. Do not skip ahead.
- After completing each task, build the project and confirm it compiles before moving on.
- Do not add features not listed in the spec. Solve the task at hand, then stop.
- When you are right about something the user challenges, say so clearly. Do not reflexively agree.

## Language and Framework

- This is a **C++17** project using **JUCE 8.0.7** via CMake FetchContent.
- **No Objective-C. No AppKit. No Cocoa. No `#import`.** Everything goes through JUCE.
- The existing files in `DSP/` are complete and correct. **Do not modify them.**
- `MyAmpPlugin.cpp/.h` is the existing VST3/AU plugin. **Do not modify it.**
- Do not introduce audio or DSP libraries other than JUCE without asking first.
- Use `juce::jlimit`, `juce::jmax`, `juce::jmin` instead of manual clamping.
- Prefer `const` and `constexpr`. Use `std::unique_ptr` or `std::shared_ptr` instead of raw owning pointers.
- Use `std::atomic` for any state shared between the audio thread and other threads.

## The .tiny-ai Folder — Agent Rules

The `.tiny-ai/` folder is managed by the **user**, not the agent.

As an agent you MAY:
- Read any file in `.tiny-ai/` at any time
- Write new spec files to `.tiny-ai/specs/` when explicitly asked to create a spec

As an agent you MUST NOT:
- Delete any file or folder inside `.tiny-ai/`
- Modify an existing `.tiny-ai/` file unless the user explicitly asks you to update it
- Move files out of `.tiny-ai/`

If the user asks you to delete or reorganise `.tiny-ai/` contents, that is fine — act on their explicit instruction. Never do it on your own initiative.

All new project source code goes in `Source/`, `tests/`, or the project root — never in `.tiny-ai/`.

## Real-Time Safety (Audio Thread)

The audio thread is `AudioEngine::audioDeviceIOCallbackWithContext()` and everything it calls. On the audio thread:
- **No memory allocation** (`new`, `malloc`, `std::vector::push_back` that grows)
- **No locks** (`std::mutex::lock`, `std::lock_guard`)
- **No file I/O, no logging, no `printf`**
- Use `juce::AbstractFifo` to pass data from audio thread to UI thread
- Use `juce::LinearSmoothedValue` for parameter changes to avoid zipper noise
- Pre-allocate all buffers in `prepareToPlay()`, not in `processBlock()`

## JUCE Patterns

- Parameters → `juce::AudioProcessorValueTreeState` (APVTS)
- Serialisation → `juce::ValueTree`
- Audio device → `juce::AudioDeviceManager`
- Waveforms → `juce::AudioThumbnail` / `juce::AudioThumbnailCache`
- MIDI storage → `juce::MidiMessageSequence`
- Audio file I/O → `juce::AudioFormatManager` + `juce::WavAudioFormat`
- Timer-driven repaints → subclass `juce::Timer`, call `startTimerHz(30)`

## Build Commands

```bash
# Configure (only needed after CMakeLists.txt changes)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"

# Build everything
cmake --build build --parallel

# Run tests
ctest --test-dir build --output-on-failure

# Run the DAW
open build/MyDAW_artefacts/Release/Standalone/MyDAW.app

# Verify the existing plugin still works (must always pass)
auval -v aufx Amp1 MPlg
```

Always build after every C++ change before presenting results.

## File Layout

New source files go in:
- `Source/Engine/` — AudioEngine, TransportEngine, MixerEngine, TrackProcessor, EffectChain, MidiEngine, PluginHost
- `Source/Effects/` — AmpSimulatorFX, DistortionFX, OverdriveFX, PhaserFX, ChorusFX, DelayFX
- `Source/Instruments/` — SubtractiveSynth, DrumMachine
- `Source/Model/` — Project, Track, Clip, ProjectSerializer
- `Source/UI/` — all Component subclasses
- `tests/` — CTest unit tests

## Testing

- Every non-trivial class gets a CTest unit test in `tests/<ClassName>Test.cpp`.
- Use the minimal hand-rolled harness pattern from existing tests in `tests/`.
- Add each test to `CMakeLists.txt` using the `add_dsp_test()` helper already defined there.
- Run `ctest --output-on-failure` after every task. All tests must pass before moving to the next task.
- `auval -v aufx Amp1 MPlg` must continue to pass throughout.

## Existing Code to Reuse

| What | Where | How to use |
|---|---|---|
| Full amp DSP chain | `DSP/DSPCoreImpl.h` → `DigitalAmpDSP` | Instantiate in `AmpSimulatorFX`, call `initialize()` and `process()` |
| All DSP stages | `DSP/*.h` | Used internally by `DigitalAmpDSP` — do not use directly |

## Dark UI Theme

```cpp
namespace DawTheme {
    const juce::Colour background    { 0xFF1A1A1A };
    const juce::Colour panelBg       { 0xFF242424 };
    const juce::Colour border        { 0xFF2D2D2D };
    const juce::Colour accent        { 0xFFE8A020 }; // amber
    const juce::Colour textPrimary   { 0xFFE0E0E0 };
    const juce::Colour textSecondary { 0xFF888888 };
    const juce::Colour clipAudio     { 0xFF3A6EA5 }; // blue
    const juce::Colour clipMidi      { 0xFF5A9E5A }; // green
    const juce::Colour meterGreen    { 0xFF4CAF50 };
    const juce::Colour meterYellow   { 0xFFFFEB3B };
    const juce::Colour meterRed      { 0xFFF44336 };
}
```
