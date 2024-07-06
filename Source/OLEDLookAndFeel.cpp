/*
  ==============================================================================

    OLEDLookAndFeel.cpp
    Created: 6 Jul 2024 12:46:48am
    Author:  MK

  ==============================================================================
*/

#include "OLEDLookAndFeel.h"

namespace juce
{
    OLEDScreen::OLEDScreen()
    {
        depthLabel.setText("0", juce::dontSendNotification);
        levelLabel.setText("0", juce::dontSendNotification);

        addAndMakeVisible(depthLabel);
        addAndMakeVisible(levelLabel);
    }

    void OLEDScreen::paint(juce::Graphics& g)
    {
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
        g.drawImage(juce::ImageCache::getFromMemory(BinaryData::pedal_base_darkestblue_lcd_png, BinaryData::pedal_base_darkestblue_lcd_pngSize), getLocalBounds().toFloat());
        g.setColour(juce::Colours::white);

        int outerLeft = 285, outerTop = 89, outerRight = 472, outerBottom = 200;
        int innerLeft = outerLeft + 4;
        int innerTop = outerTop + 5;
        int innerRight = outerRight - 4;
        int innerBottom = outerBottom - 5;
        int innerWidth = innerRight - innerLeft;
        int innerHeight = innerBottom - innerTop;
        g.setColour(juce::Colours::lightgrey);
        float cornerSize = 5.0f;
        g.drawRoundedRectangle(juce::Rectangle<float>(innerLeft, innerTop, innerWidth, innerHeight), cornerSize, 2.0f);

        g.setFont(customFont.withHeight(18.0f));
        juce::String text = "TREMOLO";
        g.drawText(text, innerLeft, innerTop + 1, (innerRight - innerLeft), 20, juce::Justification::horizontallyCentred);

        juce::String text2 = "DEPTH:";
        g.drawText(text2, innerLeft + 4, innerTop + 19, customFont.getStringWidth(text2), 18, juce::Justification::centredLeft);

        juce::String text3 = "EFFECT LEVEL:";
        g.drawText(text3, innerLeft + 4, innerTop + 37, customFont.getStringWidth(text3), 18, juce::Justification::centredLeft);

        juce::String text4 = "WAVEFORM:";
        g.drawText(text4, innerLeft + 4, innerTop + 55, customFont.getStringWidth(text4), 18, juce::Justification::centredLeft);

        juce::String text5 = "DIRECT LEVEL:";
        g.drawText(text5, innerLeft + 4, innerTop + 73, customFont.getStringWidth(text5), 18, juce::Justification::centredLeft);
    }

    void OLEDScreen::setDepthLabel(const juce::String& text)
    {
        depthLabel.setText(text, juce::dontSendNotification);
    }

    void OLEDScreen::setLevelLabel(const juce::String& text)
    {
        levelLabel.setText(text, juce::dontSendNotification);
    }

    void OLEDScreen::setFont(const juce::Font& font)
    {
        customFont = font;
        depthLabel.setFont(font);
        levelLabel.setFont(font);
    }
}
