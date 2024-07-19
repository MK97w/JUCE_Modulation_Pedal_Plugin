/*
  ==============================================================================

    KnobLookAndFeel.h
    Created: 6 Jul 2024 12:17:42am
    Author:  MK

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Knob : public juce::Slider, juce::LookAndFeel_V4
{
public:
    Knob(juce::Image sourceImage);
    void drawRotarySlider(juce::Graphics& g,
        int x,
        int y,
        int width,
        int height,
        float sliderPosProportional,
        float rotaryStartAngle,
        float rotaryEndAngle,
        juce::Slider& slider);
    juce::Image img;
};