#include "vibrato.h"


std::unique_ptr<juce::AudioProcessorParameterGroup> Vibrato::createVibratoParameterGroup()
{
    auto params = std::make_unique<juce::AudioProcessorParameterGroup>("vibratoGroup", "Vibrato", "|");

    params->addChild(std::make_unique<juce::AudioParameterFloat>("_Vibrato_Intensity1", "Intensity",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    params->addChild(std::make_unique<juce::AudioParameterFloat>("_Vibrato_Elevel2", "Effect Level",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    params->addChild(std::make_unique<juce::AudioParameterFloat>("_Vibrato_Dlevel3", "Direct Level",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    params->addChild(std::make_unique<juce::AudioParameterFloat>("_Vibrato_Depth4", "Depth",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    params->addChild(std::make_unique<juce::AudioParameterChoice>("Vibrato_WF5", "Waveform",
        juce::StringArray{ "Sine", "Triangle", "Sawtooth", "InverseSawtooth", "Square", "Pulse", "RampUp", "RampDown", "Random", "Harmonic" }, 0));
    params->addChild(std::make_unique<juce::AudioParameterFloat>("Vibrato_Rate6", "Rate",
        juce::NormalisableRange<float>(0.1f, 10.0f, 0.01f), 1.0f));
    params->addChild(std::make_unique<juce::AudioParameterInt>("Vibrato_Complexity7", "Complexity", 1, 10, 5));
    params->addChild(std::make_unique<juce::AudioParameterFloat>("Vibrato_InputSensitivity8", "Input Sensitivity",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    params->addChild(std::make_unique<juce::AudioParameterFloat>("Vibrato_InitPhase9", "Initial Phase",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f));

    return params;
}

void Vibrato::fetchParametersFromAPVTS(const juce::AudioProcessorParameterGroup& group)
{
    //implement
}

void Vibrato::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    inverseSampleRate = 1.0f / static_cast<float>(sampleRate);
    delayBufferSamples = 2 * samplesPerBlock; // Allow up to 2x buffer size for delay
    delayBufferChannels = 2; // Stereo
    delayBuffer.setSize(delayBufferChannels, delayBufferSamples);
    delayBuffer.clear();
    delayWritePosition = 0;
}

void Vibrato::processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer&)
{
    /*const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();

    const float intensity = *apvts.getRawParameterValue("_Vibrato_Intensity1");
    const float effectLevel = *apvts.getRawParameterValue("_Vibrato_Elevel2");
    const float directLevel = *apvts.getRawParameterValue("_Vibrato_Dlevel3");
    const float depth = *apvts.getRawParameterValue("_Vibrato_Depth4");
    const int waveformIndex = static_cast<int>(*apvts.getRawParameterValue("Vibrato_WF5"));
    const float rate = *apvts.getRawParameterValue("Vibrato_Rate6");

    lfo::waveform currentWaveform = static_cast<lfo::waveform>(waveformIndex);

    for (int channel = 0; channel < numChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer(channel);

        for (int sampleIndex = 0; sampleIndex < numSamples; ++sampleIndex)
        {
            lfoPhase += rate * inverseSampleRate;
            if (lfoPhase >= 1.0f)
                lfoPhase -= 1.0f;

            float lfoValue = lfo()(lfoPhase, depth, currentWaveform);

            int delaySamples = static_cast<int>(intensity * lfoValue);
            if (delaySamples > delayBufferSamples)
                delaySamples = delayBufferSamples;

            delayBuffer.setSample(channel, delayWritePosition, channelData[sampleIndex] * directLevel);

            int readPosition = (delayWritePosition - delaySamples + delayBufferSamples) % delayBufferSamples;
            float delayedSample = delayBuffer.getSample(channel, readPosition);

            channelData[sampleIndex] += delayedSample * effectLevel;

            ++delayWritePosition %= delayBufferSamples;
        }
    }*/
}