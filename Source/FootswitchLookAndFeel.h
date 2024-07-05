/*
  ==============================================================================

    FootswitchLookAndFeel.h
    Created: 6 Jul 2024 12:18:15am
    Author:  MK

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace juce
{

  class FootswitchButton :public ImageButton, public ChangeBroadcaster
  {
   public:
       FootswitchButton(Image idleImage, Image pressedImage);
       void mouseDown(const MouseEvent& event) override;
       void mouseUp(const MouseEvent& event) override;
       void redraw();

   private:
       Image idle, pressed;
       bool isDown = false;
  };
}
