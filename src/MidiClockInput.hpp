#pragma once

#include <vector>
#include <chrono>
#include <functional>

class MidiClockInput {
public:
    MidiClockInput();
    void handleTimingMessage(double bufOffsetMs);
    void handleStartMessage();

    std::function<void(const float newTempo)> onTempoChange;

private:
    unsigned int DELTA_COUNT = 100;
    double lastKnownTempo = 0.0;
    std::vector<double> deltas = std::vector<double>(DELTA_COUNT);
    unsigned int deltaPointer = 0;
    bool startIsArmed = false;
    std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds> previousNow;
    const std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds> zero;
};