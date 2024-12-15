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
    vibratoGroup->addChild(std::make_unique<juce::AudioParameterInt>("Vibrato_WF5", "WAVEFORM",  1, 4, 1));
    vibratoGroup->addChild(std::make_unique<juce::AudioParameterFloat>("Vibrato_Rate6", "RATE", 0.1f, 10.0f, 5.0f));
    vibratoGroup->addChild(std::make_unique<juce::AudioParameterInt>("Vibrato_Complexity7", "COMPLEXITY", 1, 4, 1));
	return vibratoGroup;
}

void Vibrato::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    const double smoothTime = 1e-3;

/*

    paramWidth.reset(sampleRate, smoothTime);
    paramFrequency.reset(sampleRate, smoothTime);
    paramWaveform.reset(sampleRate, smoothTime);
    paramInterpolation.reset(sampleRate, smoothTime);


    float maxDelayTime = paramWidth.maxValue;
    delayBufferSamples = (int)(maxDelayTime * (float)sampleRate) + 1;
    if (delayBufferSamples < 1)
        delayBufferSamples = 1;

    delayBufferChannels = getTotalNumInputChannels();
    delayBuffer.setSize(delayBufferChannels, delayBufferSamples);
    delayBuffer.clear();

    delayWritePosition = 0;
    lfoPhase = 0.0f;
    inverseSampleRate = 1.0f / (float)sampleRate;*/
}