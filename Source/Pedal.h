/*
  ==============================================================================

    Pedal.h
    Created: 13 Jul 2024 4:54:32pm
    Author:  MK

  ==============================================================================
*/

#pragma once
#include "KnobLookAndFeel.h" 
#include "FootswitchLookAndFeel.h" 
#include "OLEDLookAndFeel.h" 

class Pedal
{
private:
    std::array<Knob, 6> knobs; // Array of 6 knobs
    std::array<FootswitchButton, 3> footswitches; // Array of 3 footswitches
    //OLEDLookAndFeel OLEDDisplay; // An instance of the OLED display class
};