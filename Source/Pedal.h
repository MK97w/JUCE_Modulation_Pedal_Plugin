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
#include "OLEDPageFactory.h" 

#include "PluginProcessor.h" 

#include "layout.h"


class Pedal : public juce::Component, /*public juce::Button::Listener,*/ public juce::Slider::Listener
{
public:
   Pedal(Modulation_Pedal_PluginAudioProcessor&);
   ~Pedal();
   void paint(juce::Graphics& g) override;
   void resized() override;
   juce::Rectangle<int> getBounds() { return pedalBounds; };
   std::unordered_map<juce::String, std::vector<juce::RangedAudioParameter*>> parameterGroups;


private:
   Modulation_Pedal_PluginAudioProcessor& processor;
   juce::AudioProcessorValueTreeState& pedalAPVTS;
   
   std::unique_ptr<OLEDPage> currentOLEDPage;
   PageFactory pageFactory;

   juce::Image pedalBaseImage;
   juce::Rectangle<int> pedalBounds; 
   std::array<std::unique_ptr<FootswitchButton>, 3> footswitches; // this looks fine but maybe i should find a way to give them a name or individualize them
   std::array<std::unique_ptr<Knob>, 6> knobs;
   juce::TextButton upButton, downButton, editButton, exitButton;
   

   std::unordered_map<int, std::string> effects;


   void initializeFootswitches();
   void initializeKnobs();
   void initializeComponents();
   void resizeComponents();
   void resizeFootswitches();
   void resizeKnobs();
   void initializeButtons();
   void resizeButtons();
   void downButtonClicked();
   void upButtonClicked();
   void editButtonClicked();
   void exitButtonClicked();
   void initializeParameterGroups();


   void sliderValueChanged(juce::Slider* slider) override;
   void sliderDragStarted(juce::Slider* slider) override;
   void sliderDragEnded(juce::Slider* slider) override;


   int currentAPVTSIndex = 0;
   int displayOffset = 0;




   //THESE ARE THE BOUNDS OF DISPLAY PART: MAKE THEM JUCE::RECTANGLE
   int outerLeft = 285, outerTop = 89, outerRight = 472, outerBottom = 200;
   int innerLeft = outerLeft + 4;
   int innerTop = outerTop + 5;
   int innerBottom = outerBottom - 5;
   int innerRight = outerRight - 4;
   int innerWidth = innerRight - innerLeft; 
   int innerHeight = innerBottom - innerTop;
   float cornerSize = 5.0f;

   std::string selectedEffect;


   pageType selectedPage;


};

