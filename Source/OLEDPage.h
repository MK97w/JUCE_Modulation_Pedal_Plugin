/*
  ==============================================================================

    OLEDPage.h
    Created: 6 Jul 2024 3:41:44pm
    Author:  mert.kabukcuoglu

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
/***
*
* Notes on design:
*   - I think pages shouldnt be the listeners themselves -> they just display
*   - Base class must contain a "setPageLayout" type of function. -> Draw lines / set Title hence a variable juce::String title but this should be called only when page layout is changed.
*   - ýnstead of taking the whole apvts, create a hashamp that contains the sub-effects of apvts and pass it to display
* 
*/

//using apvtsInfo = std::unordered_map<std::string, std::vector<juce::RangedAudioParameter*>>;
 using apvtsInfo = std::vector<juce::RangedAudioParameter*> ;

class OLEDPage : public juce::Component 
{

protected:
    juce::Font customFont;
    juce::Rectangle<int> pageBounds;
    juce::String pageTitle;
    std::vector<juce::RangedAudioParameter*> apvts;

    static const juce::Font getCustomFont()
    {
        static auto typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::munro_ttf, BinaryData::munro_ttfSize);
        return juce::Font(typeface);
    }

    juce::String getPageTitle() const { return pageTitle; }
    void setPageTitle(const juce::String& newTitle) { pageTitle = newTitle; }

public:
    OLEDPage(juce::String pagetitle, apvtsInfo& info) : apvts(info), customFont(getCustomFont()), pageBounds(285, 89, 472 - 285, 200 - 89), pageTitle(pagetitle) {};
    virtual ~OLEDPage() = default;

    void setFontSize(float newSize) { customFont.setHeight(newSize); }
    void setFontBold(bool shouldBeBold) { customFont.setBold(shouldBeBold); }
    void setFontItalic(bool shouldBeItalic) { customFont.setItalic(shouldBeItalic); }
    void setFontTypeface(const juce::String& typefaceName) { customFont.setTypefaceName(typefaceName); }

    virtual void paint(juce::Graphics& g) = 0;
};


class EditPage : public OLEDPage
{
public:
    EditPage(juce::String pagetitle,apvtsInfo&);
    void paint(juce::Graphics& g) override;

private:
    int apvtsElemSize = 0;
    int currentAPVTSIndex = 0;
    int maxElemtoDisplay = 5;
    int displayOffset = 0;
    int scrollBarWidth = 5;
    int scrollBarHeight = 83;
    int scrollBarX = pageBounds.getX() + pageBounds.getWidth() - 5;
    int scrollBarY = pageBounds.getY() + 26;

};

class BasicEditPage : public OLEDPage
{
public:
    BasicEditPage(juce::String pagetitle , apvtsInfo&);
    void paint(juce::Graphics& g) override;
private:  
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
};









