cmake_minimum_required(VERSION 3.14)
project(MyDAW VERSION 1.0 LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable testing
enable_testing()

# JUCE configuration - use local submodule
set(JUCE_VERSION "8.0.7")

# Add local JUCE submodule
add_subdirectory(_deps/juce-submodule)

# Include source directories
include_directories(${PROJECT_SOURCE_DIR}/Source)

# Add subdirectories
add_subdirectory(Source)
add_subdirectory(tests)

# MyDAW standalone app target
juce_add_gui_app(MyDAW
    PRODUCT_NAME "MyDAW")

juce_generate_juce_header(MyDAW)

target_sources(MyDAW
    PRIVATE
        Source/Main.cpp
        Source/UI/MainWindow.cpp)

target_compile_definitions(MyDAW
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:MyDAW,JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:MyDAW,JUCE_VERSION>"
        JUCE_MAC)

target_link_libraries(MyDAW
    PRIVATE
        juce::juce_core
        juce::juce_gui_basics
        juce::juce_audio_processors
    PUBLIC
        juce::juce_recommended_config_flags)