#include "SyncEditor.hpp"

SyncEditor::SyncEditor(SyncProcessor& p)
        : AudioProcessorEditor(&p), syncProcessor(p)
{
    playStopButton.setBounds(0, 0, 75, 30);
    updatePlayStopButtonText();
    addAndMakeVisible(playStopButton);

    playStopButton.onClick = [this]{
        if (syncProcessor.isMaster())
        {
            syncProcessor.togglePlayStop();
            updatePlayStopButtonText();
        }
    };

    masterSlaveButton.setBounds(75, 0, 75, 30);
    updateMasterSlaveButtonText();
    addAndMakeVisible(masterSlaveButton);

    masterSlaveButton.onClick = [this]{
        syncProcessor.toggleMasterSlave();
        updateMasterSlaveButtonText();

        if (syncProcessor.isMaster())
        {
            playStopButton.setAlpha(1);
        }
        else
        {
            if (syncProcessor.isPlaying())
            {
                syncProcessor.togglePlayStop();
            }

            playStopButton.setAlpha(0.5);
        }
    };

    setSize(320, 200);
    setWantsKeyboardFocus(true);
}

bool SyncEditor::keyPressed(const juce::KeyPress &)
{
    syncProcessor.togglePlayStop();
    return true;
}

void SyncEditor::updatePlayStopButtonText()
{
    playStopButton.setButtonText(syncProcessor.isPlaying() ? "stop" : "play");
}

void SyncEditor::updateMasterSlaveButtonText()
{
    masterSlaveButton.setButtonText(syncProcessor.isMaster() ? "master" : "slave");
}
