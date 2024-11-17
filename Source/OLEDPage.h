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

using apvtsInfo = std::unordered_map<std::string, std::vector<juce::RangedAudioParameter*>>;


class OLEDPage : public juce::Component 
{

protected:
    juce::Font customFont;
    juce::Rectangle<int> pageBounds;
    juce::String pageTitle;
	apvtsInfo& apvts;

    static const juce::Font getCustomFont()
    {
        static auto typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::munro_ttf, BinaryData::munro_ttfSize);
        return juce::Font(typeface);
    }

public:
    OLEDPage(apvtsInfo& info) : apvts(info), customFont(getCustomFont()), pageBounds(285, 89, 472 - 285, 200 - 89) {};
    virtual ~OLEDPage(); 

    void setFontSize(float newSize) { customFont.setHeight(newSize); }
    void setFontBold(bool shouldBeBold) { customFont.setBold(shouldBeBold); }
    void setFontItalic(bool shouldBeItalic) { customFont.setItalic(shouldBeItalic); }
    void setFontTypeface(const juce::String& typefaceName) { customFont.setTypefaceName(typefaceName); }

    void paint(juce::Graphics& g) override;
};


class EditPage : public OLEDPage
{
public:
    EditPage(apvtsInfo&);
    void paint(juce::Graphics& g) override;
};

class BasicEditPage : public OLEDPage
{
public:
    BasicEditPage(apvtsInfo&);
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









