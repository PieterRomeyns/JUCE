cmake_minimum_required(VERSION 3.14)
project(Effects)

add_library(Effects STATIC
    DelayFX.cpp
)

target_include_directories(Effects PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_link_libraries(Effects PUBLIC
    juce::juce_audio_processors
    juce::juce_core
)