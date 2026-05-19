#include "DelayFX.h"
#include "../deps/juce-submodule/extras/Projucer/Source/JUCEHeader.h"
#include "catch2/catch_test_macros.hpp"

TEST_CASE("DelayFX processes block correctly") {
    DelayFX processor;

    // Set parameters
    processor.apvts.getParameter(DelayFX::paramTime)->setValueNotifyingHost(500.0);
    processor.apvts.getParameter(DelayFX::paramFeedback)->setValueNotifyingHost(0.5f);
    processor.apvts.getParameter(DelayFX::paramMix)->setValueNotifyingHost(0.5f);

    // Prepare for playback
    processor.prepareToPlay(44100.0, 128);

    // Create test buffer
    juce::AudioBuffer<float> buffer(1, 128);
    float* data = buffer.getWritePointer(0);

    // Fill with sine wave
    for (int i = 0; i < 128; ++i) {
        data[i] = std::sin(2 * M_PI * 440.0f * i / 44100.0f);
    }

    // Process block
    processor.processBlock(buffer, {});

    // Verify output
    for (int i = 0; i < 128; ++i) {
        REQUIRE(-1.0f <= buffer.getSample(0, i) && buffer.getSample(0, i) <= 1.0f);
    }
}