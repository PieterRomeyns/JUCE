#pragma once
#include <JuceHeader.h>

class MainWindow : public juce::DocumentWindow
{
public:
    MainWindow();
    ~MainWindow() override;

private:
    std::unique_ptr<juce::Component> contentComponent;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
};