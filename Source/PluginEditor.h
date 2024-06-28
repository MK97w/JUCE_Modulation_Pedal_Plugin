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
class Modulation_Pedal_PluginAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                                     public juce::Slider::Listener
{
public:
    Modulation_Pedal_PluginAudioProcessorEditor (Modulation_Pedal_PluginAudioProcessor&);
    ~Modulation_Pedal_PluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider* slider) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Modulation_Pedal_PluginAudioProcessor& audioProcessor;

    juce::CustomizedLook::Knob leftKnob, rightKnob_1, rightKnob_2, rightKnob_3, rightKnob_4, rightKnob_5;
    juce::CustomizedLook::FootswitchButton leftFootswitch, midFootswitch, rightFootswitch;
    juce::Label rate,depth,level,param1,param2,fs1,fs2,fs3;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Modulation_Pedal_PluginAudioProcessorEditor)
};
