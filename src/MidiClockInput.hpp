#pragma once

#include <vector>
#include <functional>

class MidiClockInput {
public:
    MidiClockInput();
    void handleTimingMessage(long long frameCounter, double bufOffsetFrames, double sampleRate);
    void handleStartMessage();

    std::function<void(const float newTempo)> onTempoChange;

private:
    unsigned int DELTA_COUNT = 100;
    double lastKnownTempo = 0.0;
    std::vector<double> deltas = std::vector<double>(DELTA_COUNT);
    unsigned int deltaPointer = 0;
    bool startIsArmed = false;
    long long previousFrameCounter = 0;
};