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
   Pedal(juce::AudioProcessorValueTreeState& apvts);
   ~Pedal();
   void paint(juce::Graphics& g) override;
   void resized() override;
   juce::Rectangle<int> getBounds() { return pedalBounds; };



private:
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



   juce::Image pedalBaseImage;
   juce::Rectangle<int> pedalBounds; 
   std::array<std::unique_ptr<FootswitchButton>, 3> footswitches; // this looks fine but maybe i should find a way to give them a name or individulize them
   std::array<std::unique_ptr<Knob>, 6> knobs;

   juce::TextButton upButton, downButton;

   juce::AudioProcessorValueTreeState& pedalAPVTS;

   juce::String paramsString;
   void updateParamsString();


   int apvtsElemSize = 0;
   int currentAPVTSIndex = 0;
   int maxElemtoDisplay = 5;
   int displayOffset = 0;



   class CustomFontLookAndFeel : public juce::LookAndFeel_V4
   {
   public:
       CustomFontLookAndFeel()
       {
           // without this custom Fonts won't work!!
           LookAndFeel::setDefaultLookAndFeel(this);

           // This can be used as one way of setting a default font
           // setDefaultSansSerifTypeface (getCustomFont().getTypeface());
       }
       ~CustomFontLookAndFeel()
       {
           // without this custom Fonts won't work!!
           LookAndFeel::setDefaultLookAndFeel(nullptr);
       }

       static const juce::Font getCustomFont()
       {
           static auto typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::munro_ttf, BinaryData::munro_ttfSize);
           return juce::Font(typeface);
       }

       /*juce::Typeface::Ptr getTypefaceForFont(const juce::Font& f) override  --> Problem in JUCE 8??
       {
           // This can be used to fully change/inject fonts.
           // For example: return different TTF/OTF based on weight of juce::Font (bold/italic/etc)
           return getCustomFont().getTypeface();
       }*/
   private:
   } customFontLookAndFeel;


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