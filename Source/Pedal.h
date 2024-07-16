/*
  ==============================================================================

    Pedal.h
    Created: 13 Jul 2024 4:54:32pm
    Author:  MK

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <optional>
#include "KnobLookAndFeel.h" 
#include "FootswitchLookAndFeel.h" 
#include "OLEDLookAndFeel.h" 

class Pedal : public juce::Component
{
public:
    Pedal();
    ~Pedal() = default;
    void paintInBounds(juce::Graphics& g);
    juce::Rectangle<int> getBounds() { return pedalBounds; };

private:
    void paint(juce::Graphics& g) override;
    juce::Image pedalBaseImage;
    juce::Rectangle<int> pedalBounds;


  //  std::array<Knob, 6> knobs; // Array of 6 knobs
  //  std::array<FootswitchButton, 3> footswitches; // Array of 3 footswitches
  //  OLEDLookAndFeel OLEDDisplay; // An instance of the OLED display class
};