// DrumPadGrid.h
#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

namespace UI {
    class DrumPadGrid : public juce::Component {
    public:
        DrumPadGrid();
        ~DrumPadGrid() override;

        void paint(juce::Graphics&) override;
        void resized() override;
        void mouseDown(const juce::MouseEvent& event) override;
        void timerCallback() override;

    private:
        struct Pad {
            juce::Rectangle<int> bounds;
            bool isActive = false;
            juce::AudioProcessorValueTreeState* params;
        };

        std::vector<std::vector<Pad>> pads;
        juce::LinearSmoothedValue<float> sequencerStep;
        int currentStep = 0;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DrumPadGrid)
    };
}