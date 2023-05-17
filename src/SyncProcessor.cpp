#include "SyncProcessor.hpp"
#include "SyncEditor.hpp"

const juce::String SyncProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SyncProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool SyncProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool SyncProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double SyncProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SyncProcessor::getNumPrograms()
{
    return 1;
}

int SyncProcessor::getCurrentProgram()
{
    return 0;
}

void SyncProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String SyncProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void SyncProcessor::changeProgramName(int index, const juce::String &newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void SyncProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused(sampleRate, samplesPerBlock);
}

void SyncProcessor::releaseResources()
{
}

juce::AudioProcessorEditor *SyncProcessor::createEditor()
{
    return new SyncEditor(*this);
}

bool SyncProcessor::isBusesLayoutSupported(
        const juce::AudioProcessor::BusesLayout &layouts) const
{
    return true;
}

void SyncProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    juce::ignoreUnused(destData);
}

void SyncProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    juce::ignoreUnused(data, sizeInBytes);
}

void SyncProcessor::processBlock(juce::AudioSampleBuffer &buf, juce::MidiBuffer &midiBuf)
{
    midiBuf.clear();

    const float metronomeVolume = 0.7f;
    auto left = buf.getWritePointer(0);
    auto right = buf.getWritePointer(1);

    const bool isPlaying = playing.load();

    // We check if something happened on the main thread that prompts the internal sequencer to start
    // playing or to stop.
    justStartedPlaying = justStartedPlaying || (!wasPlaying && isPlaying);
    justStopped = justStopped || (wasPlaying && !isPlaying);

    wasPlaying = isPlaying;

    if (justStartedPlaying)
    {
        justStartedPlaying = false;
        // We enqueue a MIDI start event to be fired 1ms (44 frames) before
        // the next MIDI clock.
        eventAfterNFrames.frames = (int) (nextClockIsInHowManyFrames) - 44;

        if (eventAfterNFrames.frames < 0)
        {
            eventAfterNFrames.frames += framesPerClock;
        }

        eventAfterNFrames.f = [&midiBuf](int bufFrameOffset) {
            const auto startMsg = juce::MidiMessage::midiStart();
            midiBuf.addEvent(startMsg, bufFrameOffset);
//            printf("sending start at bufFrameOffset %d\n", bufFrameOffset);
        };

        internalSequencerShouldStartOnNextClock = true;
    }
    else if (justStopped)
    {
        justStopped = false;
        const auto stopMsg = juce::MidiMessage::midiStop();
        midiBuf.addEvent(stopMsg, 0);

        playStartFrame = -1;
        metronomeCounter = 3;
    }

    for (int i = 0; i < buf.getNumSamples(); i++)
    {
        const auto absoluteFramePos = frameCounter + i;

        if (eventAfterNFrames.frames >= 0)
        {
            if (--eventAfterNFrames.frames == 0)
            {
                eventAfterNFrames.f(i);
            }
        }

        // Clocks are sent at a constant interval of ~919 frames. Additionally,
        // all sequencer state transitions are quantized to this interval.
        if (floor(fmod(absoluteFramePos, framesPerClock)) == 0)
        {
            const auto clockMsg = juce::MidiMessage::midiClock();

            midiBuf.addEvent(clockMsg, i);
//            printf("Sending clock at bufFrameOffset %d\n", i);

            nextClockIsInHowManyFrames = floor(framesPerClock);

            if (internalSequencerShouldStartOnNextClock)
            {
                playStartFrame = absoluteFramePos + i;
                internalSequencerShouldStartOnNextClock = false;
            }
        }

        nextClockIsInHowManyFrames--;

        if (playStartFrame > 0)
        {
            if ((absoluteFramePos - playStartFrame) % framesPerQuarterNote == 0)
            {
                metronomeFrameIndex = 0;
                if (++metronomeCounter == 4)
                { metronomeCounter = 0; }
            }

            if (metronomeFrameIndex < metronomeSampleData.size())
            {
                auto value = metronomeSampleData[metronomeFrameIndex] * metronomeVolume * 0.00003051757812;

                // accent
                if (metronomeCounter == 0)
                { value *= 2.5; }

                left[i] = value;
                right[i] = value;
                metronomeFrameIndex++;
            }
        }
    }

    frameCounter += buf.getNumSamples();
}

void SyncProcessor::togglePlayStop()
{
    playing.store(!playing.load());
}

bool SyncProcessor::isPlaying()
{
    return playing.load();
}

void SyncProcessor::toggleMasterSlave()
{
    master.store(!master.load());
}

bool SyncProcessor::isMaster()
{
    return master.load();
}

juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new SyncProcessor();
}
