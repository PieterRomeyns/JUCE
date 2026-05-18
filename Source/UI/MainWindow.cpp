#include "MainWindow.h"

MainWindow::MainWindow()
    : DocumentWindow("My DAW", juce::Colour(0xFF1A1A1A), DocumentWindow::allButtons),
      contentComponent(std::make_unique<juce::Component>()) // Placeholder component
{
    setUsingNativeTitleBar(true);
    setContentOwned(*contentComponent, true); // Take ownership
    setResizable(true, false);
    setMinimumSize(1024, 600);
    centreWithSize(getWidth(), getHeight());
}

MainWindow::~MainWindow()
{
    // Destructor - no special cleanup needed for unique_ptr
}