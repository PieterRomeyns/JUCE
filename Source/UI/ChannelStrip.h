#pragma once
#include <JuceHeader.h>

class ChannelStrip : public juce::Component
{
public:
    ChannelStrip()
        : trackNameLabel("Track Name", "Track 1"),
          volumeSlider("Volume", juce::Slider::LinearVertical, juce::Slider::TextBoxRight),
          panSlider("Pan", juce::Slider::Rotary, juce::Slider::TextBoxRight),
          muteButton("Mute", true),
          soloButton("Solo", false)
    {
        setSize(80, 300); // Set width to 80px and reasonable height
        addAndMakeVisible(&trackNameLabel);
        addAndMakeVisible(&volumeSlider);
        addAndMakeVisible(&panSlider);
        addAndMakeVisible(&muteButton);
        addAndMakeVisible(&soloButton);

        // Add level meters (stereo)
        for (int i = 0; i < 2; ++i) {
            levelMeters.add(new juce::AudioLevelMeterComponent(i == 0 ? "L" : "R", i));
            addAndMakeVisible(levelMeters.getLast());
        }

        // Add insert slots (8)
        for (int i = 0; i < 8; ++i) {
            insertSlots.add(new juce::TextButton("Insert " + juce::String(i + 1)));
            addAndMakeVisible(insertSlots.getLast());
        }

        setOpaque(true); // For proper background drawing
    }

    void resized() override;

private:
    juce::Label trackNameLabel;
    juce::Slider volumeSlider, panSlider;
    juce::TextButton muteButton, soloButton;
    juce::OwnedArray<juce::AudioLevelMeterComponent> levelMeters;
    juce::OwnedArray<juce::TextButton> insertSlots;
};;