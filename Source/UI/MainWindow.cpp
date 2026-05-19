#include "MainWindow.h"

MainWindow::MainWindow()
    : DocumentWindow("My DAW", juce::Colour(0xFF1A1A1A), DocumentWindow::allButtons),
      contentComponent(std::make_unique<juce::Component>())
{
    setUsingNativeTitleBar(true);
    setContentOwned(contentComponent.release(), true);
    setResizable(true, true);
    centreWithSize(800, 600);
    setVisible(true);
}

MainWindow::~MainWindow()
{
    // Destructor - cleanup handled by DocumentWindow
}