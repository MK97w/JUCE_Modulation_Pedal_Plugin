/*
  ==============================================================================

    Pedal.cpp
    Created: 13 Jul 2024 4:54:32pm
    Author:  MK

  ==============================================================================
*/

#include "Pedal.h"
#include <stdexcept> 

Pedal::Pedal()
: pedalBaseImage(juce::ImageCache::getFromMemory(BinaryData::pedal_base_darkestblue_lcd_png, BinaryData::pedal_base_darkestblue_lcd_pngSize)),
  pedalBounds(0, 0, 0, 0)

{
    if (!pedalBaseImage.isNull())
    {
        pedalBounds.setSize(pedalBaseImage.getWidth(), pedalBaseImage.getHeight());
    }
	else
	{
        throw std::runtime_error("Failed to load pedal base image.");
	}
}

void Pedal::paint(juce::Graphics& g)
{
	g.drawImage(pedalBaseImage, pedalBounds.toFloat());
}


void Pedal::paintInBounds( juce::Graphics& g )
{
    paint(g);
}

