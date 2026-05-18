cmake_minimum_required(VERSION 3.14)
project(MyDAW VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# JUCE configuration
set(JUCE_VERSION "8.0.7")
set(JUCE_MODULES "core gui audio_processing audio_plugin_hosting audio_formats midi data_structures")

include(FetchContent)
FetchContent_Declare(
    juce_frameworks
    GIT_REPOSITORY https://github.com/Jucepatch/JUCE.git
    GIT_TAG        release-cpp17
)
FetchContent_MakeAvailable(juce_frameworks)

# Include source directories
include_directories(${PROJECT_SOURCE_DIR}/Source)

# Add subdirectories
add_subdirectory(Source)
add_subdirectory(tests)

# MyDAW standalone app target
add_executable(MyDAW MACOSX_BUNDLE Source/Main.cpp)
target_include_directories(MyDAW PRIVATE ${juce_frameworks_SOURCE_DIR}/modules)
target_link_libraries(MyDAW PRIVATE JUCE::juce_core JUCE::juce_gui_basic JUCE::juce_audio_processing)

# Set OSX-specific settings for standalone app
target_compile_definitions(MyDAW PRIVATE JUCE_MAC)
set_target_properties(MyDAW PROPERTIES
    MACOSX_BUNDLE_GUI_TYPE Application
    MACOSX_BUNDLE_ICON_FILE "${PROJECT_SOURCE_DIR}/Resources/MyDAW.icns"
    MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/Resources/Info.plist"
)