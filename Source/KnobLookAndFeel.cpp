/*
  ==============================================================================

    KnobLookAndFeel.cpp
    Created: 6 Jul 2024 12:17:42am
    Author:  MK

  ==============================================================================
*/

#include "KnobLookAndFeel.h"

    Knob::Knob(juce::Image sourceImage) : img(sourceImage) {};

    void Knob::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, Slider& s)
    {
        const double rotation = (s.getValue() - s.getMinimum()) / (s.getMaximum() - s.getMinimum());
        const int frames = img.getHeight() / img.getWidth();
        const int frameId = (int)ceil(rotation * ((double)frames - 1.0));
        const float radius = juce::jmin(width / 2.0f, height / 2.0f);
        const float centerX = x + width * 0.5f;
        const float centerY = y + height * 0.5f;
        const float rx = centerX - radius - 1.0f;
        const float ry = centerY - radius;

        g.drawImage(img,
            (int)rx,
            (int)ry,
            2 * (int)radius,
            2 * (int)radius,
            0,
            frameId * img.getWidth(),
            img.getWidth(),
            img.getWidth());
    }
