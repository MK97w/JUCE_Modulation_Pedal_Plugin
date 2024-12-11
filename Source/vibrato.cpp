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
    vibratoGroup->addChild(std::make_unique<juce::AudioParameterInt>("_Vibrato_WF4", "WAVEFORM",  1, 4, 1));
    vibratoGroup->addChild(std::make_unique<juce::AudioParameterFloat>("Vibrato_Rate5", "RATE", 0.1f, 10.0f, 5.0f));
    vibratoGroup->addChild(std::make_unique<juce::AudioParameterInt>("Vibrato_Complexity6", "COMPLEXITY", 1, 4, 1));
    vibratoGroup->addChild(std::make_unique<juce::AudioParameterFloat>("Vibrato_G", "Vibrato G", 0.0f, 1.0f, 0.5f));

	return vibratoGroup;
}