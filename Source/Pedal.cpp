/*
  ==============================================================================

    Pedal.cpp
    Created: 13 Jul 2024 4:54:32pm
    Author:  MK

  ==============================================================================
*/

#include "Pedal.h"
#include <stdexcept> 
#include <memory> 

Pedal::Pedal()
: pedalBaseImage(juce::ImageCache::getFromMemory(BinaryData::pedal_base_darkestblue_lcd_png,
                                                BinaryData::pedal_base_darkestblue_lcd_pngSize)),
  pedalBounds(0, 0, 0, 0)

{
    if (pedalBaseImage.isNull())
    {
        throw std::runtime_error("Failed to load pedal base image.");
    }

    pedalBounds.setSize(pedalBaseImage.getWidth(), pedalBaseImage.getHeight());
    leftFootswitch.setImg(juce::ImageCache::getFromMemory(BinaryData::left_footswitch_idle_png, BinaryData::left_footswitch_idle_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::left_footswitch_pressed_png, BinaryData::left_footswitch_idle_pngSize));
    
  
}

void Pedal::paint(juce::Graphics& g)
{
	g.drawImage(pedalBaseImage, pedalBounds.toFloat());
}

void Pedal::resized()
{
    leftFootswitch.setBounds(155, 420, 90, 90);
}
