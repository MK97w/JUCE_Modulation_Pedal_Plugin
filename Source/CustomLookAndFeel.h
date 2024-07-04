/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 22 Jun 2024 9:45:31pm
    Author:  MK

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace juce
{
    class CustomizedLook
    {
    public:
        struct FootswitchButton : ImageButton, ChangeBroadcaster 
        {

            FootswitchButton(Image idleImage, Image pressedImage);
            void mouseDown(const MouseEvent&)override;
            void mouseUp(const MouseEvent&)override;
            void redraw();
            
            bool isDown{ false };
            Image idle,pressed;
        };

        struct Knob : Slider, LookAndFeel_V4
        {
            Knob(Image sourceImage);
            void drawRotarySlider(Graphics&, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, Slider&);
            Image img;
        };
    };



}
