#pragma once

#include <vector>
#include <functional>

class MidiClockInput {
public:
    MidiClockInput();
    void handleTimingMessage(double framePos, double sampleRate);
    void handleStartMessage();
    void handleStopMessage();

    std::function<void(const float newTempo)> onTempoChange;
    std::function<void()> onStart;
    std::function<void()> onStop;

private:
    unsigned int DELTA_COUNT = 100;
    double lastKnownTempo = 0.0;
    std::vector<double> deltas = std::vector<double>(DELTA_COUNT);
    unsigned int deltaPointer = 0;
    bool startIsArmed = false;
    double previousFrameCounter = 0;
};