#include "SyncEditor.hpp"

SyncEditor::SyncEditor(SyncProcessor& p)
        : AudioProcessorEditor(&p), syncProcessor(p)
{
    playStopButton.setBounds(0, 0, 75, 30);
    updatePlayStopButtonText();
    addAndMakeVisible(playStopButton);

    playStopButton.onClick = [this]{
//        if (syncProcessor.isMaster())
//        {
            syncProcessor.togglePlayStop();
            updatePlayStopButtonText();
//        }
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

    tempoLabel.setBounds(150, 0, 50, 30);
    tempoLabel.setEditable(true);
    tempoLabel.setComponentID("tempo");

    updateTempoButtonText();
    addAndMakeVisible(tempoLabel);

    tempoLabel.onEditorHide = [this]{
        auto text = tempoLabel.getText();
        float newTempo = 0.0;
        try { newTempo = std::stof(text.toStdString()); }
        catch (const std::exception&) { return; }
        syncProcessor.setTempo(newTempo);
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

void SyncEditor::updateTempoButtonText()
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << syncProcessor.getTempo();
    std::string tempoString = ss.str();

    tempoLabel.setText(tempoString, juce::dontSendNotification);
}
