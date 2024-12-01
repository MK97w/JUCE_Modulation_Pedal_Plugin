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


class Pedal : public juce::Component, public juce::Button::Listener, public juce::Slider::Listener
{
public:
   Pedal(juce::AudioProcessorValueTreeState& apvts);
   ~Pedal();
   void paint(juce::Graphics& g) override;
   void resized() override;
   juce::Rectangle<int> getBounds() { return pedalBounds; };



private:

   std::unique_ptr<OLEDPage> currentOLEDPage;
   PageFactory pageFactory;

   juce::Image pedalBaseImage;
   juce::Rectangle<int> pedalBounds; 
   std::array<std::unique_ptr<FootswitchButton>, 3> footswitches; // this looks fine but maybe i should find a way to give them a name or individulize them
   std::array<std::unique_ptr<Knob>, 6> knobs;
   juce::TextButton upButton, downButton, editButton, exitButton;
   juce::AudioProcessorValueTreeState& pedalAPVTS;
   std::unordered_map<std::string, std::vector<juce::RangedAudioParameter*>> parameterGroups;
   juce::String paramsString;
  
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

   void sliderValueChanged(juce::Slider* slider) override;
   void buttonClicked(juce::Button* button) override;


   int apvtsElemSize = 0;
   int currentAPVTSIndex = 0;
   int maxElemtoDisplay = 5;
   int displayOffset = 0;
   bool isEditPage = false;



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

};



/*
class Pedal : public juce::Component {
public:
    Pedal();

private:
    juce::Image loadImage(const void* data, int dataSize);
    std::unique_ptr<FootswitchButton> initFootswitch(const juce::Image& idleImage, const juce::Image& pressedImage, int x, int y, int width, int height);

    std::unique_ptr<FootswitchButton> leftFootswitch;
    std::unique_ptr<FootswitchButton> midFootswitch;
    std::unique_ptr<FootswitchButton> rightFootswitch;
    // Other members...
};





juce::Image Pedal::loadImage(const void* data, int dataSize) {
    return juce::ImageCache::getFromMemory(data, dataSize);
}

std::unique_ptr<FootswitchButton> Pedal::initFootswitch(const juce::Image& idleImage, const juce::Image& pressedImage, int x, int y, int width, int height) {
    auto footswitch = std::make_unique<FootswitchButton>(idleImage, pressedImage);
    footswitch->setBounds(x, y, width, height);
    addAndMakeVisible(*footswitch);
    return footswitch;
}

Pedal::Pedal() {
    if (pedalBaseImage.isNull()) {
        throw std::runtime_error("Failed to load pedal base image.");
    }
    pedalBounds.setSize(pedalBaseImage.getWidth(), pedalBaseImage.getHeight());

    // Load images
    auto leftIdleImage = loadImage(BinaryData::leftFootswitchIdle_png, BinaryData::leftFootswitchIdle_pngSize);
    auto leftPressedImage = loadImage(BinaryData::leftFootswitchPressed_png, BinaryData::leftFootswitchPressed_pngSize);
    auto midIdleImage = loadImage(BinaryData::midFootswitchIdle_png, BinaryData::midFootswitchIdle_pngSize);
    auto midPressedImage = loadImage(BinaryData::midFootswitchPressed_png, BinaryData::midFootswitchPressed_pngSize);
    auto rightIdleImage = loadImage(BinaryData::rightFootswitchIdle_png, BinaryData::rightFootswitchIdle_pngSize);
    auto rightPressedImage = loadImage(BinaryData::rightFootswitchPressed_png, BinaryData::rightFootswitchPressed_pngSize);

    // Initialize footswitch buttons
    leftFootswitch = initFootswitch(leftIdleImage, leftPressedImage, 155, 420, 90, 90);
    midFootswitch = initFootswitch(midIdleImage, midPressedImage, 430, 420, 90, 90);
    rightFootswitch = initFootswitch(rightIdleImage, rightPressedImage, 705, 420, 90, 90);
}






*/