/*
  ==============================================================================

    vibrato.cpp
    Created: 10 Dec 2024 5:56:28pm
    Author:  mert.kabukcuoglu

  ==============================================================================
*/

#include "vibrato.h"

std::unique_ptr<juce::AudioProcessorParameterGroup> Vibrato::createVibratoParameterGroup()
{
    auto vibratoGroup = std::make_unique<juce::AudioProcessorParameterGroup>("vibrato", "Vibrato", "|");
    vibratoGroup->addChild(std::make_unique<juce::AudioParameterFloat>("_Vibrato_Intensity1", "INTENSITY", 0.1f, 10.0f, 5.0f));
    vibratoGroup->addChild(std::make_unique<juce::AudioParameterFloat>("_Vibrato_Elevel2", "EFFECT LEVEL", 0.0f, 1.0f, 0.5f));
    vibratoGroup->addChild(std::make_unique<juce::AudioParameterFloat>("_Vibrato_Dlevel3", "DIRECT LEVEL", 0.0f, 1.0f, 0.5f));
    vibratoGroup->addChild(std::make_unique<juce::AudioParameterFloat>("_Vibrato_Depth4", "DEPTH", 0.0f, 1.0f, 0.5f));
    vibratoGroup->addChild(std::make_unique<juce::AudioParameterInt>("Vibrato_WF5", "WAVEFORM",  1, 10, 1));
    vibratoGroup->addChild(std::make_unique<juce::AudioParameterFloat>("Vibrato_Rate6", "RATE", 0.1f, 10.0f, 5.0f));
    vibratoGroup->addChild(std::make_unique<juce::AudioParameterInt>("Vibrato_Complexity7", "COMPLEXITY", 0.0f, 100.0f, 50.0f));
    vibratoGroup->addChild(std::make_unique<juce::AudioParameterFloat>("Vibrato_InputSensitivity8", "SENSITIVITY", 0.0f, 100.0f, 0.0f));
    vibratoGroup->addChild(std::make_unique<juce::AudioParameterFloat>("Vibrato_InitPhase9", "PHASE", 0.0f, 345.0f, 0.0f));
	return vibratoGroup;
}

void Vibrato::processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midiMessages)
{
    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();

    // Fetch parameters from the parameter group
    const float intensity = *vibratoGroup->getParameter("_Vibrato_Intensity1");
    const float effectLevel = *vibratoGroup->getParameter("_Vibrato_Elevel2");
    const float directLevel = *vibratoGroup->getParameter("_Vibrato_Dlevel3");
    const float depth = *vibratoGroup->getParameter("_Vibrato_Depth4");
    const int waveformIndex = static_cast<int>(*vibratoGroup->getParameter("Vibrato_WF5"));
    const float rate = *vibratoGroup->getParameter("Vibrato_Rate6");
    const int complexity = static_cast<int>(*vibratoGroup->getParameter("Vibrato_Complexity7"));
    const float sensitivity = *vibratoGroup->getParameter("Vibrato_InputSensitivity8");
    const float phase = *vibratoGroup->getParameter("Vibrato_InitPhase9");

    // Set the LFO waveform based on the index (you can update this according to your needs)
    lfo::waveform currentWaveform = static_cast<lfo::waveform>(waveformIndex);

    // LFO parameters
    float lfoFreq = rate;  // LFO frequency in Hz
    float lfoDepth = depth; // LFO depth in normalized range

    // Loop over all channels
    for (int channel = 0; channel < numChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer(channel);

        // Process each sample in the channel
        for (int sampleIndex = 0; sampleIndex < numSamples; ++sampleIndex)
        {
            // Update LFO phase increment based on rate
            lfoPhase += lfoFreq / inverseSampleRate;
            if (lfoPhase >= 1.0f)
                lfoPhase -= 1.0f;

            // Calculate LFO value (using LFO operator)
            float lfoValue = lfo()(lfoPhase, lfoDepth, currentWaveform);

            // Apply vibrato effect
            int delaySamples = static_cast<int>(intensity * lfoValue);
            if (delaySamples > delayBufferSamples)
                delaySamples = delayBufferSamples;

            // Write current sample to delay buffer (direct level processing)
            delayBuffer.setSample(channel, delayWritePosition, channelData[sampleIndex] * directLevel);

            // Apply the vibrato using the delay buffer
            int readPosition = delayWritePosition - delaySamples;
            if (readPosition < 0)
                readPosition += delayBufferSamples;

            // Get the delayed sample and apply effect level
            float delayedSample = delayBuffer.getSample(channel, readPosition);
            channelData[sampleIndex] += delayedSample * effectLevel;

            // Increment write position in delay buffer, wrapping if necessary
            ++delayWritePosition;
            if (delayWritePosition >= delayBufferSamples)
                delayWritePosition = 0;
        }
    }
}