cmake_minimum_required(VERSION 3.14)
project(Tests)

enable_testing()

# Add test executable
add_executable(DelayFXTest DelayFXTest.cpp)

# Link test executable with required libraries
target_link_libraries(DelayFXTest PRIVATE 
    Effects
    juce::juce_core
    juce::juce_audio_processors
)

# Add test
add_test(NAME DelayFXTest COMMAND DelayFXTest)