/*
  ==============================================================================

    OLEDPage.cpp
    Created: 6 Jul 2024 3:41:44pm
    Author:  mert.kabukcuoglu

  ==============================================================================
*/
#include "OLEDPage.h"

BasicEditPage::BasicEditPage(juce::String pagetitle, apvtsInfo& info ) : OLEDPage(pagetitle,info)
{
}

void BasicEditPage::paint(juce::Graphics& g)
{
    g.drawRoundedRectangle(juce::Rectangle<float>(innerLeft, innerTop, innerWidth, innerHeight), cornerSize, 2.0f); //simple edit page
    g.setFont(customFont);
    g.setFont(18.5f);
    g.drawText(getPageTitle(), innerLeft, innerTop + 1, (innerRight - innerLeft), 20, juce::Justification::horizontallyCentred);
    g.setFont(18.5f);
    juce::StringArray lines;
    int lineHeight = g.getCurrentFont().getHeight();
	int yPosition = innerTop + 22;
    /*for (int i = 0; i < 4; ++i)
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
    }*/

    for (const auto param : apvts)
    {
        if (auto name = param->getParameterID(); name.startsWithChar('_') == false)
            continue;
        else
        {
            juce::String paramValue = param->getCurrentValueAsText();
            //g.drawText(param->getName(100) + ": " + paramValue, 20, yPosition + 20, getWidth() - 40, 20, juce::Justification::left);
            g.drawText(param->getName(100) + ": ", innerLeft + 6, yPosition, innerWidth, lineHeight, juce::Justification::centredLeft);
            g.drawText(paramValue, innerRight - 35, yPosition, innerWidth, lineHeight, juce::Justification::centredLeft);
            yPosition += lineHeight;
        }
    }


}