#pragma once

#include "juce_audio_processors/juce_audio_processors.h"
#include "SyncProcessor.hpp"

class TempoLabel : public juce::Label, public juce::Timer {
    void timerCallback() override;

    juce::TextEditor* createEditorComponent() override
    {
        juce::TextEditor* const ed = Label::createEditorComponent();
        ed->setInputRestrictions(5, "0123456789.");
        return ed;
    }
};

class SyncEditor : public juce::AudioProcessorEditor {
public:
    explicit SyncEditor(SyncProcessor&);

    bool keyPressed(const juce::KeyPress& keyPress) override;

    void updateTempoButtonText();

private:
    SyncProcessor& syncProcessor;
    juce::TextButton playStopButton;
    juce::TextButton masterSlaveButton;
    TempoLabel tempoLabel;

    void updatePlayStopButtonText();
    void updateMasterSlaveButtonText();
};