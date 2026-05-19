// DrumPadGrid.cpp
#include "DrumPadGrid.h"
#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>

namespace UI {
    DrumPadGrid::DrumPadGrid()
        : sequencerStep(0.0f, 0.0f, 16.0f)
    {
        // Initialize 4x4 pad grid
        pads.resize(4);
        for (auto& row : pads) {
            row.resize(4);
            for (auto& pad : row) {
                pad.params = new juce::AudioProcessorValueTreeState();
            }
        }

        startTimerHz(30);
    }

    DrumPadGrid::~DrumPadGrid() {
        for (auto& row : pads) {
            for (auto& pad : row) {
                delete pad.params;
            }
        }
    }

    void DrumPadGrid::paint(juce::Graphics& g) {
        g.fillAll(DawTheme::background);

        // Draw grid
        g.setColour(DawTheme::accent);
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                auto& pad = pads[row][col];
                g.drawRoundedRectangle(pad.bounds, 5.0f, 2.0f);

                // Draw active state
                if (pad.isActive) {
                    g.setOpacity(0.5f);
                    g.fillRoundedRectangle(pad.bounds, 5.0f);
                    g.setOpacity(1.0f);
                }
            }
        }

        // Draw sequencer steps below each pad
        g.setColour(DawTheme::textPrimary);
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                auto& pad = pads[row][col];
                auto x = pad.bounds.getX() + 5;
                auto y = pad.bounds.getY() + pad.bounds.getHeight() + 5;
                g.drawFittedText("" + juce::String(currentStep == (row*4 + col) ? "*":""), 
                                juce::Rectangle<int>(x, y, 30, 20),
                                juce::Justification::centred,
                                1);
            }
        }
    }

    void DrumPadGrid::resized() {
        auto area = getBounds();
        int padWidth = (area.getWidth() - 40) / 4;
        int padHeight = (area.getHeight() - 60) / 4;

        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                auto x = 20 + col * padWidth;
                auto y = 20 + row * padHeight;
                pads[row][col].bounds = juce::Rectangle<int>(x, y, padWidth, padHeight);
            }
        }
    }

    void DrumPadGrid::mouseDown(const juce::MouseEvent& event) {
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                auto& pad = pads[row][col];
                if (pad.bounds.contains(event.getPosition())) {
                    pad.isActive = !pad.isActive;
                    repaint();
                    break;
                }
            }
        }
    }

    void DrumPadGrid::timerCallback() {
        currentStep = sequencerStep.getNextValue();
        // Update sequencer display
        repaint();
    }
}