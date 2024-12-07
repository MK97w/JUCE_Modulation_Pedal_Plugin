/*
  ==============================================================================

    OLEDPage.h
    Created: 6 Jul 2024 3:41:44pm
    Author:  mert.kabukcuoglu

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "layout.h"

using apvtsInfo = std::vector<juce::RangedAudioParameter*> ;

class OLEDPage : public juce::Component , public juce::LookAndFeel_V4
{

protected:
    juce::Font customFont;
    juce::Rectangle<int> pageBounds;
    juce::String pageTitle;
    std::vector<juce::RangedAudioParameter*> apvts;
    int currentAPVTSIndex = 0;
    int displayOffset = 0;
    int maxElemtoDisplay = 5;


    static const juce::Font getCustomFont()
    {
        static auto typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::munro_ttf, BinaryData::munro_ttfSize);
        return juce::Font(typeface);
    }

	juce::Typeface::Ptr getTypefaceForFont(const juce::Font& f) override //this is wont working probably in my juce version
    {
        return getCustomFont().getTypefacePtr();
    }

    juce::String getPageTitle() const { return pageTitle; }
    void setPageTitle(const juce::String& newTitle) { pageTitle = newTitle; }

public:
    OLEDPage(juce::String pagetitle, apvtsInfo& info) : apvts(info), customFont(getCustomFont()),
                                                        pageBounds(285, 89, 472 - 285, 200 - 89), pageTitle(pagetitle)
    {};
    
    virtual ~OLEDPage() { setLookAndFeel(nullptr); };
    void setFontSize(float newSize) { customFont.setHeight(newSize); }
    void setFontBold(bool shouldBeBold) { customFont.setBold(shouldBeBold); }
    void setFontItalic(bool shouldBeItalic) { customFont.setItalic(shouldBeItalic); }
    void setFontTypeface(const juce::String& typefaceName) { customFont.setTypefaceName(typefaceName); }
    void set_currentAPVTSIndex(int index) { currentAPVTSIndex = index; };
    void set_displayOffset(int offset) { displayOffset = offset; };
    int getMaximumElementsToDisplay() { return maxElemtoDisplay; };
    virtual void paint(juce::Graphics& g) = 0;
};


class EditPage : public OLEDPage
{
public:
    EditPage(juce::String pagetitle,apvtsInfo&);
    ~EditPage() { setLookAndFeel(nullptr); };
    void paint(juce::Graphics& g) override;
   
   

private:

    int scrollBarWidth = 5;
    int scrollBarHeight = 83;
    int scrollBarX = pageBounds.getX() + pageBounds.getWidth() - 5;
    int scrollBarY = pageBounds.getY() + 26;
    const juce::String editPageTitle = "EDIT ";
};

class BasicEditPage : public OLEDPage
{
public:
    BasicEditPage(juce::String pagetitle , apvtsInfo&);
	~BasicEditPage() { setLookAndFeel(nullptr); };
    void paint(juce::Graphics& g) override;
private:  
    //adding these to the base class would be better
    int innerLeft = pageBounds.getX() + 4;
    int innerTop = pageBounds.getY() + 5;
    int innerBottom = pageBounds.getBottom() - 5;
    int innerRight = pageBounds.getRight() - 4;
    int innerWidth = innerRight - innerLeft;
    int innerHeight = innerBottom - innerTop;
    float cornerSize = 5.0f;
};

class PresetPage : public OLEDPage
{
	//will be implemented later :) will contain the rate information
};









