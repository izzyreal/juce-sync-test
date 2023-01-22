#pragma once

#include <juce_audio_utils/juce_audio_utils.h>

struct EventAfterNFrames {
    int frames = -1;
    std::function<void(int)> f = [](int bufFrameOffset){};
};

class SyncProcessor : public juce::AudioProcessor
{
public:
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;

    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;

    void getStateInformation(juce::MemoryBlock &destData) override;

    void setStateInformation(const void *data, int sizeInBytes) override;

    juce::AudioProcessorEditor *createEditor() override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;

    bool producesMidi() const override;

    bool isMidiEffect() const override;

    double getTailLengthSeconds() const override;

    int getNumPrograms() override;

    int getCurrentProgram() override;

    void setCurrentProgram(int index) override;

    const juce::String getProgramName(int index) override;

    void changeProgramName(int index, const juce::String &newName) override;

    void processBlock(juce::AudioSampleBuffer &, juce::MidiBuffer &) override;

    bool hasEditor() const override
    { return true; }

    void playStop();

private:
    std::vector<short> metronomeSampleData{0, 2047, 4092, 6145, 8186, 10239, 12284, 14329, 14333, 14330, 14330, 14332,
                                           14330, 14331, 14330, 14330, 14333, 14329, 14333, 14328, 14335, 14329, 14331,
                                           14331, 14331, 14333, 14329, 14333, 14329, 14331, 14330, 14331, 14331, 14331,
                                           14330, 14330, 14333, 14331, 14330, 14331, 14331, 14329, 14331, 14331, 14331,
                                           14331, 14330, 14330, 14334, 14328, 1};

    // 120BPM
    const float framesPerClock = 918.75;
    const short framesPerQuarterNote = 22050;

    int frameCounter = 0;
    char metronomeFrameIndex = 127;
    float nextClockIsInHowManyFrames = 0.f;

    // Counts to 4 to provide accents
    char metronomeCounter = 3;

    int playStartFrame = -1;
    std::atomic<bool> playing{false };
    bool wasPlaying{false};

    bool internalSequencerShouldStartOnNextClock = false;
    bool justStartedPlaying = false;
    bool justStopped = false;

    EventAfterNFrames eventAfterNFrames;
};
