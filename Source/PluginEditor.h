/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomLookAndFeel.h"

//==============================================================================
/**
*/
class Modulation_Pedal_PluginAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    Modulation_Pedal_PluginAudioProcessorEditor (Modulation_Pedal_PluginAudioProcessor&);
    ~Modulation_Pedal_PluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Modulation_Pedal_PluginAudioProcessor& audioProcessor;

    juce::CustomizedLook::Knob leftKnob;
    juce::CustomizedLook::Knob rightKnob_1;
    juce::CustomizedLook::Knob rightKnob_2;
    juce::CustomizedLook::Knob rightKnob_3;
    juce::CustomizedLook::Knob rightKnob_4;
    juce::CustomizedLook::Knob rightKnob_5;

    juce::CustomizedLook::FootswitchButton leftFootswitch;
    juce::CustomizedLook::FootswitchButton midFootswitch;
    juce::CustomizedLook::FootswitchButton rightFootswitch;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Modulation_Pedal_PluginAudioProcessorEditor)
};
