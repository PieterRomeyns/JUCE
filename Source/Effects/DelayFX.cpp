#include "DelayFX.h"

const juce::Identifier DelayFX::paramTime = juce::Identifier("time");
const juce::Identifier DelayFX::paramFeedback = juce::Identifier("feedback");
const juce::Identifier DelayFX::paramMix = juce::Identifier("mix");

DelayFX::DelayFX()
    : apvts(*this, nullptr, "Parameters", {})
{
}

DelayFX::~DelayFX() {}

void DelayFX::prepareToPlay(double sampleRate, int samplesPerBlock) {
    delayInSamples = static_cast<int>(juce::jlimit(10, 2000, 
        static_cast<int>(1000.0))); // Placeholder for now
    feedback = 0.5f;
    mix = 0.5f;

    delayBuffer = std::make_unique<juce::AudioBuffer<float>>(1, delayInSamples);
}

void DelayFX::releaseResources() {
    delayBuffer = nullptr;
}

void DelayFX::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    auto numChannels = buffer.getNumChannels();
    auto numSamples = buffer.getNumSamples();

    if (delayBuffer == nullptr)
        return;

    // Clear delay buffer
    delayBuffer->clear();

    for (int channel = 0; channel < numChannels; ++channel) {
        float* channelData = buffer.getWritePointer(channel);
        float* delayChannelData = delayBuffer->getWritePointer(0);

        for (int sample = 0; sample < numSamples; ++sample) {
            // Read from delay buffer
            float delayedSample = delayChannelData[sample % delayInSamples];

            // Calculate wet signal
            float wetSignal = delayedSample * feedback;

            // Mix dry and wet signals
            float total = juce::jlimit(-1.0f, 1.0f, channelData[sample] + wetSignal);
            channelData[sample] = juce::jmap(total, -1.0f, 1.0f, -1.0f * mix, 1.0f * mix);

            // Write to delay buffer
            delayChannelData[sample % delayInSamples] = channelData[sample];
        }
    }
}