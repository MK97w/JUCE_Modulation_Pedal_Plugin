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
    initializeComponents();
}

void Pedal::paint(juce::Graphics& g)
{
	g.drawImage(pedalBaseImage, pedalBounds.toFloat());
}

void Pedal::resized()
{
    resizeComponents();
}

void Pedal::initializeFootswitches()
{
	footswitches[0] = std::make_unique<FootswitchButton>(juce::ImageCache::getFromMemory(BinaryData::left_footswitch_idle_png,
		                                                 BinaryData::left_footswitch_idle_pngSize),
		                                                 juce::ImageCache::getFromMemory(BinaryData::left_footswitch_pressed_png,
			                                             BinaryData::left_footswitch_pressed_pngSize));


    footswitches[1] = std::make_unique<FootswitchButton>(juce::ImageCache::getFromMemory(BinaryData::mid_footswitch_idle_png,
                                                         BinaryData::mid_footswitch_idle_pngSize),
                                                         juce::ImageCache::getFromMemory(BinaryData::mid_footswitch_pressed_png,
                                                         BinaryData::mid_footswitch_pressed_pngSize));

    footswitches[2] = std::make_unique<FootswitchButton>(juce::ImageCache::getFromMemory(BinaryData::right_footswitch_idle_png,
                                                         BinaryData::right_footswitch_idle_pngSize),
                                                         juce::ImageCache::getFromMemory(BinaryData::right_footswitch_pressed_png,
                                                         BinaryData::right_footswitch_pressed_pngSize));

    for (const auto& footswitch : footswitches)
        addAndMakeVisible(*footswitch);

}
void Pedal::resizeFootswitches()
{
    //this should be relational with image size and wrt each other so that i can iterate through and set bounds
    footswitches[0]->setBounds(155, 420, 90, 90);
    footswitches[1]->setBounds(430, 420, 90, 90);
    footswitches[2]->setBounds(705, 420, 90, 90);
}

void Pedal::initializeComponents()
{
    initializeFootswitches();
}
void Pedal::resizeComponents()
{
    resizeFootswitches();
}