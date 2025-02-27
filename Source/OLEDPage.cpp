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
    if (currentAPVTSIndex == -1)
    {
        g.setColour(juce::Colours::lightgrey);
        g.fillRect(innerLeft+2, innerTop+3 ,innerWidth-4, 16);
        g.setColour(juce::Colours::black);
        g.drawText(getPageTitle().toUpperCase(), innerLeft, innerTop + 1, (innerRight - innerLeft), 20, juce::Justification::horizontallyCentred);
    }
	else
	{
		g.setColour(juce::Colours::white);
		g.drawText(getPageTitle().toUpperCase(), innerLeft, innerTop + 1, (innerRight - innerLeft), 20, juce::Justification::horizontallyCentred);
	}
    g.setColour(juce::Colours::white);
    g.setFont(18.0f);
    int lineHeight = g.getCurrentFont().getHeight();
	int yPosition = innerTop + 22;
    for (int i = 0; i < apvts.size(); ++i)
    {
        auto param = apvts[i];
        if (auto name = param->getParameterID(); name.startsWithChar('_') == false)
            continue;
        else
        {

            if (currentAPVTSIndex == i)
            {
                g.setColour(juce::Colours::lightgrey);
                g.fillRect(innerLeft + 2, yPosition - 1 , innerWidth - 4, 18);
                g.setColour(juce::Colours::black);
                juce::String paramValue = param->getCurrentValueAsText();
                g.drawText(param->getName(100) + ": ", innerLeft + 6, yPosition, innerWidth, lineHeight, juce::Justification::centredLeft);
                g.drawText(paramValue, innerRight - 35, yPosition, innerWidth, lineHeight, juce::Justification::centredLeft);
                yPosition += lineHeight;
            }
            else
            {
                g.setColour(juce::Colours::white);
                juce::String paramValue = param->getCurrentValueAsText();
                g.drawText(param->getName(100) + ": ", innerLeft + 6, yPosition, innerWidth, lineHeight, juce::Justification::centredLeft);
                g.drawText(paramValue, innerRight - 35, yPosition, innerWidth, lineHeight, juce::Justification::centredLeft);
                yPosition += lineHeight;
            }
        }
    }


}

EditPage::EditPage(juce::String pagetitle, apvtsInfo& info) : OLEDPage(pagetitle, info)
{
}

void EditPage::paint(juce::Graphics& g) 
{

    g.setColour(juce::Colours::white);
    g.drawRect(scrollBarX, scrollBarY, scrollBarWidth, scrollBarHeight);
    int totalItems = apvts.size(); 
    int visibleItems = maxElemtoDisplay;

    if (totalItems > 0 && visibleItems > 0)
    {
        float scrollBarRatio = static_cast<float>(visibleItems) / totalItems;
        int whiteHeight = static_cast<int>(scrollBarHeight * scrollBarRatio);
        int maxDisplayOffset = totalItems - visibleItems;
        int whiteY = scrollBarY + static_cast<int>((scrollBarHeight - whiteHeight) * (static_cast<float>(displayOffset) / maxDisplayOffset));

        g.setColour(juce::Colours::lightgrey);
        g.fillRect(scrollBarX, whiteY, scrollBarWidth, whiteHeight);
    }

    g.drawLine(pageBounds.getX(), pageBounds.getY() + 22, pageBounds.getX() + pageBounds.getWidth(), pageBounds.getY() + 22, 2.0f);
    auto ft = getCustomFont().boldened();
    ft.setExtraKerningFactor(0.1f);
    g.setFont(ft);
    g.setFont(15.5f);
    g.drawText(editPageTitle, pageBounds.getX() + 2, pageBounds.getY() + 2, 8, 20, juce::Justification::left);
    g.setFont(getCustomFont());
    g.setFont(16.5f);
    juce::String text2 = "[" + getPageTitle().toUpperCase() + "] ";
    g.drawText(text2, pageBounds.getRight() - getCustomFont().getStringWidth(text2)-4, pageBounds.getY() + 2, getCustomFont().getStringWidth(text2), 20, juce::Justification::left);
    g.setFont(18.0f);
    int lineHeight = g.getCurrentFont().getHeight();
    for (int i = 0; i < maxElemtoDisplay; ++i)
    {
        juce::String paramName = apvts[i + displayOffset]->getName(100);
        juce::String paramValue = apvts[i + displayOffset]->getCurrentValueAsText(); 
        if (displayOffset == 0)
        {

                if (currentAPVTSIndex == i)
                {
                    g.setColour(juce::Colours::lightgrey);
                    g.fillRect(pageBounds.getX() + 4, pageBounds.getY() + 24 + i * lineHeight, (pageBounds.getWidth() - 13), lineHeight);

                    g.setColour(juce::Colours::black);
                    g.drawText(paramName +": ", pageBounds.getX() + 6, pageBounds.getY() + 24 + i * lineHeight, pageBounds.getWidth() - 4, lineHeight, juce::Justification::centredLeft);
                    g.drawText(paramValue, pageBounds.getRight() - 43, pageBounds.getY() + 24 + i * lineHeight, pageBounds.getWidth() - 4, lineHeight, juce::Justification::centredLeft);
                }
                else
                {
                    g.setColour(juce::Colours::white);
                    g.drawText(paramName + ": ", pageBounds.getX() + 4, pageBounds.getY() + 24 + i * lineHeight, pageBounds.getWidth() -4 , lineHeight, juce::Justification::centredLeft);
                    g.drawText(paramValue, pageBounds.getRight() - 43, pageBounds.getY() + 24 + i * lineHeight, pageBounds.getWidth() -4 , lineHeight, juce::Justification::centredLeft);
                }
        }
        else
        {
            if (currentAPVTSIndex - displayOffset == i) //index - offset
            {
                g.setColour(juce::Colours::lightgrey);
                g.fillRect(pageBounds.getX() + 4, pageBounds.getY() + 24 + i * lineHeight, (pageBounds.getWidth() - 11), lineHeight);

                g.setColour(juce::Colours::black);
                g.drawText(paramName + ": ", pageBounds.getX() + 6, pageBounds.getY() + 24 + i * lineHeight, pageBounds.getWidth() -4, lineHeight, juce::Justification::centredLeft);
                g.drawText(paramValue, pageBounds.getRight() - 43, pageBounds.getY() + 24 + i * lineHeight, pageBounds.getWidth() -4, lineHeight, juce::Justification::centredLeft); 
            }
            else
            {
                g.setColour(juce::Colours::white);
                g.drawText(paramName + ": ", pageBounds.getX() + 6, pageBounds.getY() + 24 + i * lineHeight, pageBounds.getWidth() -4 , lineHeight, juce::Justification::centredLeft);
                g.drawText(paramValue, pageBounds.getRight() - 43, pageBounds.getY() + 24 + i * lineHeight, pageBounds.getWidth() -4, lineHeight, juce::Justification::centredLeft);
            }
        }

    }

}