/*
  ==============================================================================

    KnobLookAndFeel.h
    Created: 6 Jul 2024 12:17:42am
    Author:  MK

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace juce
{
    class Knob : Slider, LookAndFeel_V4
    {
    public:
        Knob(Image sourceImage);
        void drawRotarySlider(Graphics& g,
            int x,
            int y,
            int width,
            int height,
            float sliderPosProportional,
            float rotaryStartAngle,
            float rotaryEndAngle,
            Slider& slider);
        Image img;
    };
} 