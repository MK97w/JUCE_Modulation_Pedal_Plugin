/*
  ==============================================================================

    OLEDPage.h
    Created: 6 Jul 2024 3:41:44pm
    Author:  mert.kabukcuoglu

  ==============================================================================
*/
#pragma once

#include <JuceHeader.h>

namespace juce
{
    class OLEDPage
    {
    public:
        OLEDPage();
        void setFont(const Font& font);
        void addLabel(const String& text, const Rectangle<int>& bounds);
        void clearLabels();
        void addToComponent(Component& parent);

    private:
        juce::OwnedArray<Label> labels;


    public:
         class CustomFontLookAndFeel : public juce::LookAndFeel_V4
         {
         public:
            CustomFontLookAndFeel()
            {
                LookAndFeel::setDefaultLookAndFeel(this);
            }
            static const juce::Font getCustomFont()
            {
                static auto typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::munro_ttf, BinaryData::munro_ttfSize);
                return juce::Font(typeface);
            }
          private:
          }customFontLookAndFeel;
    };
}
