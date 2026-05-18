#include <JuceHeader.h>

class MyDAWApplication : public juce::JUCEApplication {
public:
    const juce::String getApplicationName() override { return "MyDAW"; }
    const juce::String getApplicationVersion() override { return "1.0"; }
    bool moreThanOneInstanceAllowed() override { return false; }

    void initialise(const juce::String& commandLine) override {
        // Create the main window and show it
        mainWindow.reset(new MainWindow(getApplicationName()));
    }

    void shutdown() override {
        mainWindow = nullptr; // (deletes our window)
    }

    void systemRequest(Quit) override {
        quit();
    }

private:
    std::unique_ptr<juce::DocumentWindow> mainWindow;
};

class MainWindow : public juce::DocumentWindow {
public:
    MainWindow(juce::StringName title) 
        : juce::DocumentWindow(title, juce::Colours::black, juce::DocumentWindow::allButtons)
    {
        setContentOwned(new juce::Label("Hello", "MyDAW Standalone App"), true);
        setResizable(true, true);
        centreWithSize(getWidth(), getHeight());
        setVisible(true);
    }

    void closeButtonPressed() override {
        JUCEApplication::getInstance()->systemRequest(Quit);
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
};

AUTO_START_APPLICATION(MyDAWApplication)