#include "ChannelStrip.h"

ChannelStrip::ChannelStrip()
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

void ChannelStrip::resized()
{
    const int margin = 5;
    const int buttonHeight = 20;
    const int meterHeight = 15;
    const int sliderWidth = getWidth() - margin * 2;

    // Track name label at the top
    trackNameLabel.setBounds(margin, margin,
                             getWidth() - margin * 2,
                             buttonHeight);
    trackNameLabel.setFont(juce::Font(buttonHeight * 0.8f));
    trackNameLabel.setJustificationType(juce::Justification::centredLeft);

    // Insert slots (8 small buttons stacked)
    int yPos = trackNameLabel.getBottom() + margin;
    for (int i = 0; i < insertSlots.size(); ++i) {
        insertSlots[i]->setBounds(margin, yPos,
                                 sliderWidth, buttonHeight * 0.8f);
        insertSlots[i]->setButtonText("I" + juce::String(i + 1));
        insertSlots[i]->setColour(juce::TextButton::buttonColourId,
                                 juce::Colours::grey); // Small, subtle buttons
        yPos += buttonHeight * 0.8f + margin * 0.5f;
    }

    // Pan knob (rotary)
    panSlider.setBounds(margin, yPos + margin,
                        sliderWidth, sliderWidth);
    panSlider.setRange(-1.0, 1.0, 0.0);
    yPos = panSlider.getBottom() + margin;

    // Level meters (stereo side by side)
    for (int i = 0; i < levelMeters.size(); ++i) {
        levelMeters[i]->setBounds(margin + i * (sliderWidth / 2 + margin),
                                 yPos,
                                 sliderWidth / 2,
                                 meterHeight);
    }
    yPos = levelMeters.getLast()->getBottom() + margin;

    // Volume fader (vertical)
    volumeSlider.setBounds(margin, yPos + margin,
                           sliderWidth, getHeight() - yPos - margin * 2);
    volumeSlider.setRange(0.0, 1.0, 0.0);

    // Mute and solo buttons side by side at bottom
    muteButton.setBounds(margin, getHeight() - buttonHeight * 2 - margin * 2,
                         sliderWidth / 2 - margin, buttonHeight);
    soloButton.setBounds(sliderWidth / 2 + margin, getHeight() - buttonHeight * 2 - margin * 2,
                         sliderWidth / 2 - margin, buttonHeight);
}