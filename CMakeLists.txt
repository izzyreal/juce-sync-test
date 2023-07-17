cmake_minimum_required(VERSION 3.13)

include(FetchContent)

project(synctest VERSION 0.0.1)

FetchContent_Declare(JUCE
  GIT_REPOSITORY https://github.com/izzyreal/JUCE.git
  GIT_TAG        develop
  SOURCE_DIR     ${CMAKE_SOURCE_DIR}/deps/JUCE
)

FetchContent_MakeAvailable(JUCE)

juce_add_plugin(synctest
    BUNDLE_ID org.example.synctest
    COMPANY_NAME Izmar
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT TRUE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    PLUGIN_MANUFACTURER_CODE Izmr
    PLUGIN_CODE SncT
    FORMATS VST3 Standalone
    PRODUCT_NAME "synctest")

set(_src_root_path "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(
  GLOB_RECURSE _source_list
  LIST_DIRECTORIES false
  RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
  "${_src_root_path}/*.c*"
  "${_src_root_path}/*.h*"
)

target_sources(synctest PRIVATE ${_source_list})

target_compile_definitions(synctest PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_DISPLAY_SPLASH_SCREEN=0)

target_link_libraries(synctest PRIVATE
    juce::juce_audio_utils
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)
