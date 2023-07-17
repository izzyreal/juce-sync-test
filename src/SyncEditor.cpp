#include "SyncEditor.hpp"

SyncEditor::SyncEditor(SyncProcessor& p)
        : AudioProcessorEditor(&p), syncProcessor(p)
{
    setSize(300, 300);
    setResizable(true, false);
    getConstrainer()->setFixedAspectRatio(1.0);
    setWantsKeyboardFocus(true);
    juce::TopLevelWindow::getTopLevelWindow(0)->setUsingNativeTitleBar(true);
}

bool SyncEditor::keyPressed(const juce::KeyPress &)
{
    syncProcessor.playStop();
    return true;
}

void SyncEditor::paint(juce::Graphics& g)
{
    DBG("Width: " + juce::String(getWidth()) + ", height: " + juce::String(getHeight()));
    return;
    const int SPACE = 10 * (getWidth() / 300.f);
    for (int i = 0; i < 30; i++)
    {
        g.drawLine(i * SPACE, 0, i*SPACE , 500);
        g.drawLine(0, i * SPACE, 500, i*SPACE);
    }
}