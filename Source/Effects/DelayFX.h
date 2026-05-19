#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>

class DelayFX : public juce::AudioProcessor {
public:
    DelayFX();
    ~DelayFX() override;

    //==============================================================================
    const juce::String getName() const override { return "Delay FX"; }
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;

    //==============================================================================
    // APVTS parameters
    static const juce::Identifier paramTime;
    static const juce::Identifier paramFeedback;
    static const juce::Identifier paramMix;

    juce::AudioProcessorValueTreeState apvts;

private:
    std::unique_ptr<juce::AudioBuffer<float>> delayBuffer;
    int maxDelayInSamples = 0;
    float feedback = 0.0f;
    float mix = 1.0f;
    int delayInSamples = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayFX)
};