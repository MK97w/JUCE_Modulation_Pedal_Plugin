/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class Modulation_Pedal_PluginAudioProcessorEditor : public juce::AudioProcessorEditor

{
public:
    Modulation_Pedal_PluginAudioProcessorEditor (Modulation_Pedal_PluginAudioProcessor&);
    ~Modulation_Pedal_PluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    Modulation_Pedal_PluginAudioProcessor& audioProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Modulation_Pedal_PluginAudioProcessorEditor)
};
