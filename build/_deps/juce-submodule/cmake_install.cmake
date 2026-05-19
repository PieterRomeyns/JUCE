# Install script for directory: /Users/pieterromeyns/Projects/testprojects/vst-amp-plugin/_deps/juce-submodule

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/pieterromeyns/Projects/testprojects/vst-amp-plugin/build/_deps/juce-submodule/modules/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/pieterromeyns/Projects/testprojects/vst-amp-plugin/build/_deps/juce-submodule/extras/Build/cmake_install.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/JUCE-8.0.12" TYPE FILE FILES
    "/Users/pieterromeyns/Projects/testprojects/vst-amp-plugin/build/_deps/juce-submodule/JUCEConfigVersion.cmake"
    "/Users/pieterromeyns/Projects/testprojects/vst-amp-plugin/build/_deps/juce-submodule/JUCEConfig.cmake"
    "/Users/pieterromeyns/Projects/testprojects/vst-amp-plugin/_deps/juce-submodule/extras/Build/CMake/JUCECheckAtomic.cmake"
    "/Users/pieterromeyns/Projects/testprojects/vst-amp-plugin/_deps/juce-submodule/extras/Build/CMake/JUCEHelperTargets.cmake"
    "/Users/pieterromeyns/Projects/testprojects/vst-amp-plugin/_deps/juce-submodule/extras/Build/CMake/JUCEModuleSupport.cmake"
    "/Users/pieterromeyns/Projects/testprojects/vst-amp-plugin/_deps/juce-submodule/extras/Build/CMake/JUCEUtils.cmake"
    "/Users/pieterromeyns/Projects/testprojects/vst-amp-plugin/_deps/juce-submodule/extras/Build/CMake/JuceLV2Defines.h.in"
    "/Users/pieterromeyns/Projects/testprojects/vst-amp-plugin/_deps/juce-submodule/extras/Build/CMake/LaunchScreen.storyboard"
    "/Users/pieterromeyns/Projects/testprojects/vst-amp-plugin/_deps/juce-submodule/extras/Build/CMake/PIPAudioProcessor.cpp.in"
    "/Users/pieterromeyns/Projects/testprojects/vst-amp-plugin/_deps/juce-submodule/extras/Build/CMake/PIPAudioProcessorWithARA.cpp.in"
    "/Users/pieterromeyns/Projects/testprojects/vst-amp-plugin/_deps/juce-submodule/extras/Build/CMake/PIPComponent.cpp.in"
    "/Users/pieterromeyns/Projects/testprojects/vst-amp-plugin/_deps/juce-submodule/extras/Build/CMake/PIPConsole.cpp.in"
    "/Users/pieterromeyns/Projects/testprojects/vst-amp-plugin/_deps/juce-submodule/extras/Build/CMake/RecentFilesMenuTemplate.nib"
    "/Users/pieterromeyns/Projects/testprojects/vst-amp-plugin/_deps/juce-submodule/extras/Build/CMake/UnityPluginGUIScript.cs.in"
    "/Users/pieterromeyns/Projects/testprojects/vst-amp-plugin/_deps/juce-submodule/extras/Build/CMake/checkBundleSigning.cmake"
    "/Users/pieterromeyns/Projects/testprojects/vst-amp-plugin/_deps/juce-submodule/extras/Build/CMake/copyDir.cmake"
    "/Users/pieterromeyns/Projects/testprojects/vst-amp-plugin/_deps/juce-submodule/extras/Build/CMake/juce_runtime_arch_detection.cpp"
    "/Users/pieterromeyns/Projects/testprojects/vst-amp-plugin/_deps/juce-submodule/extras/Build/CMake/juce_LinuxSubprocessHelper.cpp"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/JUCE-8.0.12" TYPE DIRECTORY FILES "/Users/pieterromeyns/Projects/testprojects/vst-amp-plugin/_deps/juce-submodule/extras/Build/CMake/juce_vst3_helper")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/Users/pieterromeyns/Projects/testprojects/vst-amp-plugin/build/_deps/juce-submodule/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
