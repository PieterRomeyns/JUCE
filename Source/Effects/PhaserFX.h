#pragma once

#include <JuceHeader.h>

class PhaserFX : public juce::AudioProcessor
{
public:
    // Constructor
    PhaserFX();

    // Destructor
    ~PhaserFX() override;

    // AudioProcessor overrides
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported(const juce::AudioProcessorLayout& layouts) const override;

    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;

    // APVTS setup
    const juce::AudioProcessorValueTreeState& getParameters() override;
    const juce::StringArray& getParameterNames() override;
    const juce::StringArray& getParameterLabels() override;
    const juce::StringArray& getParameterUnits() override;

    // Parameter IDs
    enum Parameters
    {
        RATE_PARAM = 0,
        DEPTH_PARAM,
        FEEDBACK_PARAM
    };

private:
    // APVTS state
    juce::AudioProcessorValueTreeState parameters;

    // Sample rate
    double sampleRate = 0.0;

    // All-pass filter coefficients
    void updateCoefficients();

    // 4-stage all-pass filters
    struct AllPassFilter
    {
        float delayLine[2];
        int readIndex = 0;
        float coefficient = 0.0f;

        float process(float input);
    };

    AllPassFilter filters[4];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhaserFX)
};