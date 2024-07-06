/*
  ==============================================================================

    OLEDLookAndFeel.h
    Created: 6 Jul 2024 12:46:48am
    Author:  MK

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace juce
{
    class OLEDScreen : public juce::Component
    {
    public:
        OLEDScreen();
        ~OLEDScreen() = default;

        void paint(juce::Graphics& g) override;
        void setDepthLabel(const juce::String& text);
        void setLevelLabel(const juce::String& text);
        void setFont(const juce::Font& font);

    private:
        juce::Label depthLabel;
        juce::Label levelLabel;
        juce::Font customFont;

        bool set1{ false };
        bool set2{ false };

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OLEDScreen)
    };
}