/*
  ==============================================================================

    OLEDPage.cpp
    Created: 6 Jul 2024 3:41:44pm
    Author:  mert.kabukcuoglu

  ==============================================================================
*/

#include "OLEDPage.h"

// OLEDPage implementation
OLEDPage::OLEDPage()
{
 
   //customFont = CustomFontLookAndFeel::getCustomFont();
}

void OLEDPage::initializeLabels(size_t count) 
{
    labels.clear();
    for (size_t i = 0; i < count; ++i) 
    {
        labels.emplace_back();
        addAndMakeVisible(labels.back());
    }
}

void OLEDPage::setLabel(size_t index, const juce::String& text) 
{
    if (index < labels.size()) {
        labels[index].setText(text, juce::dontSendNotification);
    }
}

juce::String OLEDPage::getLabel(size_t index) const
{
    if (index < labels.size()) 
    {
        return labels[index].getText();
    }
    return {};
}

void OLEDPage::setLabelFont(size_t index, const juce::Font& font) 
{
    if (index < labels.size()) 
    {
        labels[index].setFont(font);
    }
}

// SimpleEditPage implementation
SimpleEditPage::SimpleEditPage() 
{
    initializeLabels(9); // 5 text labels + 4 slider value labels
    sliderValues.resize(4, 0.0); // Initialize slider values
}

void SimpleEditPage::paint(juce::Graphics& g) 
{
    // Example of drawing a rectangle around labels
    // You can adapt the painting code from PluginEditor.cpp
    g.setColour(juce::Colours::lightgrey);
    for (auto& label : labels) {
        auto bounds = label.getBounds().toFloat().expanded(5.0f);
        g.drawRoundedRectangle(bounds, 5.0f, 2.0f);
    }
}

void SimpleEditPage::updateSliderValue(size_t index, double value) 
{
    if (index < sliderValues.size()) 
    {
        sliderValues[index] = value;
        // Assuming the last 4 labels are for slider values
        if (index + 5 < labels.size()) 
        {
            labels[index + 5].setText(juce::String(value, 2), juce::dontSendNotification);
        }
    }
}

double SimpleEditPage::getSliderValue(size_t index) const
{
    if (index < sliderValues.size())
    {
        return sliderValues[index];
    }
    return 0.0;
}
