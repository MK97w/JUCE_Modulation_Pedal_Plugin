/*
  ==============================================================================

    FootswitchLookAndFeel.cpp
    Created: 6 Jul 2024 12:18:15am
    Author:  MK

  ==============================================================================
*/

#include "FootswitchLookAndFeel.h"

FootswitchButton::FootswitchButton(juce::Image idleImage, juce::Image pressedImage)
: idle(idleImage),
  pressed(pressedImage)
{
    redraw();    
}

void FootswitchButton::mouseDown(const juce::MouseEvent& event)
{
    isDown = true;
    //sendChangeMessage();
    redraw();
}
    
void FootswitchButton::mouseUp(const juce::MouseEvent& event)
{
    isDown = false;
    redraw();   
}

void FootswitchButton::redraw()
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