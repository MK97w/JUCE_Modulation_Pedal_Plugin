/*
  ==============================================================================

    OLEDPage.h
    Created: 6 Jul 2024 3:41:44pm
    Author:  mert.kabukcuoglu

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class OLEDPage : public juce::Component , 
                 public juce::Button::Listener ,
                 public juce::Slider::Listener
{

protected:
    juce::Font customFont;
    juce::Rectangle<int> pageBounds;
    enum class EffectType
    {
        VIBRATO,
        PHASOR,
        AUTO_WAH
    };

public:
    OLEDPage(juce::AudioProcessorValueTreeState& apvts, juce::Graphics& g);
    virtual ~OLEDPage();
    virtual void createPage(EffectType selectedEffect) = 0;
    virtual juce::String get_APVTS(EffectType) = 0;
    void paint(juce::Graphics& g) override;

};


class EditPage : public OLEDPage
{

};

class SimpleEditPage : public OLEDPage
{
};

class PresetPage : public OLEDPage
{
};









