#include "SyncEditor.hpp"

SyncEditor::SyncEditor(SyncProcessor& p)
        : AudioProcessorEditor(&p), syncProcessor(p)
{
    setSize(320, 200);
    setWantsKeyboardFocus(true);
}

bool SyncEditor::keyPressed(const juce::KeyPress &)
{
    syncProcessor.playStop();
    return true;
}
