/*
  ==============================================================================

    FootswitchLookAndFeel.h
    Created: 6 Jul 2024 12:18:15am
    Author:  MK

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class FootswitchButton :public juce::ImageButton, public juce::ChangeBroadcaster
{
public:
    FootswitchButton(juce::Image idleImage, juce::Image pressedImage);
    ~FootswitchButton();
   void mouseDown(const juce::MouseEvent& event) override;
   void mouseUp(const juce::MouseEvent& event) override;
   void redraw();

private:
   juce::Image idle, pressed;
   bool isDown = false;
};
