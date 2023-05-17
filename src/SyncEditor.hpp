#pragma once

#include "juce_audio_processors/juce_audio_processors.h"
#include "SyncProcessor.hpp"

class SyncEditor : public juce::AudioProcessorEditor {
public:
    explicit SyncEditor(SyncProcessor&);

    bool keyPressed(const juce::KeyPress& keyPress) override;

private:
    SyncProcessor& syncProcessor;
    juce::TextButton playStopButton;
    juce::TextButton masterSlaveButton;

    void updatePlayStopButtonText();
    void updateMasterSlaveButtonText();
};