/*
  ==============================================================================

    OLEDPage.cpp
    Created: 6 Jul 2024 3:41:44pm
    Author:  mert.kabukcuoglu

  ==============================================================================
*/
#include "OLEDPage.h"

OLEDPage::OLEDPage()
{
    // Assuming CustomFontLookAndFeel::getCustomFont() is accessible and appropriate here
    // customFont = CustomFontLookAndFeel::getCustomFont();
}

void OLEDPage::initializeLabels(size_t count, const std::vector<juce::Rectangle<int>>& positions)
{
    labels.clear(true); // Clear existing labels and delete them
    for (size_t i = 0; i < count; ++i)
    {
        auto* label = new juce::Label();
        labels.add(label);
        addAndMakeVisible(label);
        // If customFont is initialized properly, you can set it here
        // label->setFont(customFont);

        if (i < positions.size())
        {
            label->setBounds(positions[i]);
        }
    }
}

void OLEDPage::setLabel(size_t index, const juce::String& text)
{
    auto* label = labels[index];
    if (label != nullptr)
    {
        label->setText(text, juce::dontSendNotification);
    }
}

juce::String OLEDPage::getLabel(size_t index) const
{
    auto* label = labels[index];
    if (label != nullptr)
    {
        return label->getText();
    }
    return {};
}

void OLEDPage::setLabelFont(size_t index, const juce::Font& font)
{
    auto* label = labels[index];
    if (label != nullptr)
    {
        label->setFont(font);
    }
}

// SimpleEditPage implementation
SimpleEditPage::SimpleEditPage() 
{
    std::vector<juce::Rectangle<int>> positions = 
    {
        {10, 10, 100, 20}, {10, 40, 100, 20}, 
        {10, 70, 100, 20}, {10, 100, 100, 20}, 
        {10, 130, 100, 20},{120, 10, 100, 20}, 
        {120, 40, 100, 20}, {120, 70, 100, 20}, 
        {120, 100, 100, 20}
    };
    initializeLabels(9, positions); // 5 text labels + 4 slider value labels
    sliderValues.resize(4 * static_cast<int>(simpleEditPageContent.size()), 0.0);
}

void SimpleEditPage::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::lightgrey);
    for (auto* label : labels)
    {
        auto bounds = label->getBounds().toFloat().expanded(5.0f);
        g.drawRoundedRectangle(bounds, 5.0f, 2.0f);
    }
}

void SimpleEditPage::updateSliderValue(size_t index, double value)
{
    if (index < sliderValues.size())
    {
        sliderValues[index] = value;
        // Assuming the last 4 labels are for slider values
        if (index + 5 < static_cast<size_t>(labels.size()))
        {
            auto* label = labels[static_cast<int>(index) + 5];
            if (label != nullptr)
            {
                label->setText(juce::String(value, 2), juce::dontSendNotification);
            }
        }
    }
}

double SimpleEditPage::getSliderValue(size_t index) const
{
    if (index < sliderValues.size())
    {
        return sliderValues[index];// must use some knd of modulo of Effecttype so i can get values for each page
    }
    return 0.0;
}

void SimpleEditPage::setPageContentTo(EffectType type)
{
	auto content = simpleEditPageContent.find(type); 
	if (content != simpleEditPageContent.end())
	{
		for (size_t i = 0; i < content->second.size(); ++i)
		{
			setLabel(i, content->second[i]);
		}
	}
}

// Static member definition and initialization
const std::unordered_map<SimpleEditPage::EffectType, std::array<juce::String, 4>> SimpleEditPage::simpleEditPageContent =
{
    {SimpleEditPage::EffectType::Tremolo, {"Tremolo 1", "Tremolo 2", "Tremolo 3", "Tremolo 4"}},
    {SimpleEditPage::EffectType::Flanger, {"Flanger 1", "Flanger 2", "Flanger 3", "Flanger 4"}},
    {SimpleEditPage::EffectType::Phaser, {"Phaser 1", "Phaser 2", "Phaser 3", "Phaser 4"}}
};
