/*
  ==============================================================================

    CustomLookAndFeel.cpp
    Created: 22 Jun 2024 9:45:46pm
    Author:  MK

  ==============================================================================
*/

#include "CustomLookAndFeel.h"
namespace juce
{
    CustomizedLook::FootswitchButton::FootswitchButton(Image idleImage, Image pressedImage)
    {
        idle = idleImage;
        pressed = pressedImage;
        redraw();
    }

    void CustomizedLook::FootswitchButton::mouseDown(const MouseEvent& event)
    {
        isDown = true;
        sendChangeMessage();
        redraw();
    }
    void CustomizedLook::FootswitchButton::mouseUp(const MouseEvent& event)
    {
        isDown = false;
        redraw();
    }

    void CustomizedLook::FootswitchButton::redraw()
    {
        if (isDown)
        {
            setImages(true, true, true,
                pressed, 1.0, juce::Colours::transparentWhite,
                pressed, 1.0, juce::Colours::transparentWhite, //this is for overimage
                pressed, 1.0, juce::Colours::transparentWhite, //this is for when buttondown but ˆust add mouselistener
                0.0);
        }
        else
        {
            setImages(true, true, true,
                idle, 1.0, juce::Colours::transparentWhite,
                idle, 1.0, juce::Colours::transparentWhite, //this is for overimage
                idle, 1.0, juce::Colours::transparentWhite, //this is for when buttondown but ˆust add mouselistener
                0.0);
        }
        repaint();
    }

    CustomizedLook::Knob::Knob(Image sourceImage)
    {
        img = sourceImage;
    }
    void CustomizedLook::Knob::drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, Slider& s)
    {
        const double rotation = (s.getValue() - s.getMinimum()) / (s.getMaximum() - s.getMinimum());
        const int frames = img.getHeight() / img.getWidth();
        const int frameId = (int)ceil(rotation * ((double)frames - 1.0));
        const float radius = jmin(width / 2.0f, height / 2.0f);
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




}