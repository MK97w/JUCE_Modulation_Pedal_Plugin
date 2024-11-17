/*
  ==============================================================================

    OLEDPage.cpp
    Created: 6 Jul 2024 3:41:44pm
    Author:  mert.kabukcuoglu

  ==============================================================================
*/
#include "OLEDPage.h"

BasicEditPage::BasicEditPage(apvtsInfo& info) : OLEDPage(info)
{
	pageTitle = "Basic Edit Page";
    apvts.
}

void BasicEditPage::paint(juce::Graphics& g)
{
    //PREVIOUSLY USED FOR SIMPLE EDIT PAGE
    g.drawRoundedRectangle(juce::Rectangle<float>(innerLeft, innerTop, innerWidth, innerHeight), cornerSize, 2.0f); //simple edit page
    g.setFont(customFont);
    g.setFont(18.5f);
    juce::String text = "VIBRATO";
    g.drawText(text, innerLeft, innerTop + 1, (innerRight - innerLeft), 20, juce::Justification::horizontallyCentred);
    g.setFont(18.5f);
    juce::StringArray lines;
    int lineHeight = g.getCurrentFont().getHeight();
    for (int i = 0; i < 4; ++i)
    {
        juce::String line = lines[i];
        int colonIndex = line.indexOfChar(':');
        if (colonIndex != -1)
        {
            juce::String paramName = line.substring(0, colonIndex + 1);
            juce::String paramValue = line.substring(colonIndex + 1).trim();

            g.drawText(paramName, innerLeft + 6, innerTop + 22 + i * lineHeight, innerWidth, lineHeight, juce::Justification::centredLeft);
            g.drawText(paramValue, innerRight - 35, innerTop + 22 + i * lineHeight, innerWidth, lineHeight, juce::Justification::centredLeft);
        }
    }
}