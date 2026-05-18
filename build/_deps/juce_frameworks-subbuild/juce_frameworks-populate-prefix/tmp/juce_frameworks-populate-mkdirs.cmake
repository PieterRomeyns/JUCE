# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/Users/pieterromeyns/Projects/testprojects/vst-amp-plugin/build/_deps/juce_frameworks-src")
  file(MAKE_DIRECTORY "/Users/pieterromeyns/Projects/testprojects/vst-amp-plugin/build/_deps/juce_frameworks-src")
endif()
file(MAKE_DIRECTORY
  "/Users/pieterromeyns/Projects/testprojects/vst-amp-plugin/build/_deps/juce_frameworks-build"
  "/Users/pieterromeyns/Projects/testprojects/vst-amp-plugin/build/_deps/juce_frameworks-subbuild/juce_frameworks-populate-prefix"
  "/Users/pieterromeyns/Projects/testprojects/vst-amp-plugin/build/_deps/juce_frameworks-subbuild/juce_frameworks-populate-prefix/tmp"
  "/Users/pieterromeyns/Projects/testprojects/vst-amp-plugin/build/_deps/juce_frameworks-subbuild/juce_frameworks-populate-prefix/src/juce_frameworks-populate-stamp"
  "/Users/pieterromeyns/Projects/testprojects/vst-amp-plugin/build/_deps/juce_frameworks-subbuild/juce_frameworks-populate-prefix/src"
  "/Users/pieterromeyns/Projects/testprojects/vst-amp-plugin/build/_deps/juce_frameworks-subbuild/juce_frameworks-populate-prefix/src/juce_frameworks-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/pieterromeyns/Projects/testprojects/vst-amp-plugin/build/_deps/juce_frameworks-subbuild/juce_frameworks-populate-prefix/src/juce_frameworks-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/pieterromeyns/Projects/testprojects/vst-amp-plugin/build/_deps/juce_frameworks-subbuild/juce_frameworks-populate-prefix/src/juce_frameworks-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
