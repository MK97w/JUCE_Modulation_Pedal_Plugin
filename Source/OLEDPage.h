/*
  ==============================================================================

    OLEDPage.h
    Created: 6 Jul 2024 3:41:44pm
    Author:  mert.kabukcuoglu

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <unordered_map>
#include <array>

class OLEDPage : public juce::Component 
{
public:
    OLEDPage();
    virtual ~OLEDPage() = default;

    void setLabel(size_t index, const juce::String& text);
    juce::String getLabel(size_t index) const;
    void setLabelFont(size_t index, const juce::Font& font);
    void initializeLabels(size_t count, const std::vector<juce::Rectangle<int>>& positions);


protected:
    juce::OwnedArray<juce::Label> labels;
    juce::Font customFont;
};

class SimpleEditPage : public OLEDPage 
{
public:
    SimpleEditPage();
    virtual ~SimpleEditPage() = default;

    enum class EffectType
    {
        Tremolo,
        Flanger,
        Phaser,
        Vibrato
    };

    // Method to draw a rectangle around labels
    void paint(juce::Graphics& g) override;

    // Additional methods for SimpleEditPage
    void updateSliderValue(size_t index, double value);
    double getSliderValue(size_t index) const;
    void setPageContentTo(EffectType type);

private:
    std::vector<double> sliderValues; // Stores slider values
    const static std::unordered_map<EffectType, std::array<juce::String,5>> simpleEditPageContent;
    constexpr static float fontSize{18.0f};

};











