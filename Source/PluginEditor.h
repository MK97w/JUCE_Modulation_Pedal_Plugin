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
    bool set1{ false }, set2{ false };

    class CustomFontLookAndFeel : public juce::LookAndFeel_V4
    {
    public:
        CustomFontLookAndFeel()
        {
            // without this custom Fonts won't work!!
            LookAndFeel::setDefaultLookAndFeel(this);

            // This can be used as one way of setting a default font
            // setDefaultSansSerifTypeface (getCustomFont().getTypeface());
        }

        static const juce::Font getCustomFont()
        {
            static auto typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::munro_ttf, BinaryData::munro_ttfSize);
            return juce::Font(typeface);
        }

        /*juce::Typeface::Ptr getTypefaceForFont(const juce::Font& f) override  --> Problem in JUCE 8??
        {
            // This can be used to fully change/inject fonts.
            // For example: return different TTF/OTF based on weight of juce::Font (bold/italic/etc)
            return getCustomFont().getTypeface();
        }*/
    private:
    } customFontLookAndFeel;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Modulation_Pedal_PluginAudioProcessorEditor)
};
