#include "MidiClockInput.hpp"

#include <numeric>

MidiClockInput::MidiClockInput()
{
    const double delta120Bpm = 1.0 / 48.0;
    std::fill(deltas.begin(), deltas.end(), delta120Bpm);
}

void MidiClockInput::handleTimingMessage(long long frameCounter, double bufOffsetFrames, double sampleRate)
{
    const auto now = static_cast<double>(frameCounter) + bufOffsetFrames;

    const bool firstCall = previousFrameCounter == 0;

    if (startIsArmed)
    {
        startIsArmed = false;
        // onPlay
    }

    if (firstCall)
    {
        previousFrameCounter = now;
        return;
    }

    auto deltaInFrames = static_cast<double>(now - previousFrameCounter);

    deltas[deltaPointer++] = deltaInFrames;

    if (deltaPointer == DELTA_COUNT) deltaPointer = 0;

    previousFrameCounter = now;

    auto averageDeltaInFrames = std::reduce(deltas.begin(), deltas.end(), 0.0) / DELTA_COUNT;

//    printf("Avg delta: %f\n", averageDelta1);

    auto averageDeltaInSeconds = averageDeltaInFrames / sampleRate;

    double averageBpm = (1.0 / (averageDeltaInSeconds * 24.0)) * 60.0;

    printf("Avg BPM: %f\n", averageBpm);

    if (lastKnownTempo != averageBpm)
    {
        onTempoChange(static_cast<float>(averageBpm));
        lastKnownTempo = averageBpm;
    }
}

void MidiClockInput::handleStartMessage()
{
    previousFrameCounter = 0;
    startIsArmed = true;
}