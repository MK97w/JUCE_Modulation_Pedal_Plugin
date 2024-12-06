/*
  ==============================================================================

    OLEDLookAndFeel.h
    Created: 6 Jul 2024 12:46:48am
    Author:  MK

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "OLEDPage.h"
   
class PageFactory : public juce::Component
{
public:
    static std::unique_ptr<OLEDPage> create(pageType, const juce::String& title, apvtsInfo& info);
};
   