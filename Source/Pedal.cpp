/*
  ==============================================================================

    Pedal.cpp
    Created: 13 Jul 2024 4:54:32pm
    Author:  MK

  ==============================================================================
*/

#include "Pedal.h"
#include <stdexcept> 
#include <memory> 

Pedal::Pedal(juce::AudioProcessorValueTreeState& apvts)
: pedalBaseImage(juce::ImageCache::getFromMemory(BinaryData::pedal_base_darkestblue_lcd_png,
                BinaryData::pedal_base_darkestblue_lcd_pngSize)),
  pedalBounds(0, 0, 0, 0),
  pedalAPVTS(apvts)
{
    if (pedalBaseImage.isNull())
        throw std::runtime_error("Failed to load pedal base image.");

    pedalBounds.setSize(pedalBaseImage.getWidth(), pedalBaseImage.getHeight());
    auto xmlState = pedalAPVTS.state.toXmlString();
    DBG(xmlState);
    updateParamsString();
    initializeComponents();
}


Pedal::~Pedal()
{
	customFontLookAndFeel.setDefaultLookAndFeel(nullptr);
}

void Pedal::paint(juce::Graphics& g)
{
    int outerLeft = 285, outerTop = 89, outerRight = 472, outerBottom = 200;
    int innerLeft = outerLeft + 4;
    int innerTop = outerTop + 5;
    int innerRight = outerRight - 4;
    int innerBottom = outerBottom - 5;
    int innerWidth = innerRight - innerLeft;
    int innerHeight = innerBottom - innerTop;
    float cornerSize = 5.0f;
    g.setColour(juce::Colours::lightgrey);
    g.drawImage(pedalBaseImage, pedalBounds.toFloat());
    g.drawRoundedRectangle(juce::Rectangle<float>(innerLeft, innerTop, innerWidth, innerHeight), cornerSize, 2.0f);
    g.setFont(customFontLookAndFeel.getCustomFont());
    //g.setColour(juce::Colours::white);
   /* g.setFont(18.0f);
    g.drawText(paramsString, innerLeft + 4, innerTop + 19, 20 ,20,juce::Justification::centredLeft, 20);*/

    g.setFont(18.5f);
    juce::StringArray lines;
    lines.addLines(paramsString);

    int lineHeight = g.getCurrentFont().getHeight();
    for (int i = 0; i < lines.size(); ++i)
    {
        g.drawText(lines[i], innerLeft + 4, innerTop + 19 + i * lineHeight, innerWidth, lineHeight, juce::Justification::centredLeft);
    }

}

void Pedal::resized()
{
    resizeComponents();
}

void Pedal::initializeFootswitches()
{
	footswitches[0] = std::make_unique<FootswitchButton>(juce::ImageCache::getFromMemory(BinaryData::left_footswitch_idle_png,
		                                                 BinaryData::left_footswitch_idle_pngSize),
		                                                 juce::ImageCache::getFromMemory(BinaryData::left_footswitch_pressed_png,
			                                             BinaryData::left_footswitch_pressed_pngSize));


    footswitches[1] = std::make_unique<FootswitchButton>(juce::ImageCache::getFromMemory(BinaryData::mid_footswitch_idle_png,
                                                         BinaryData::mid_footswitch_idle_pngSize),
                                                         juce::ImageCache::getFromMemory(BinaryData::mid_footswitch_pressed_png,
                                                         BinaryData::mid_footswitch_pressed_pngSize));

    footswitches[2] = std::make_unique<FootswitchButton>(juce::ImageCache::getFromMemory(BinaryData::right_footswitch_idle_png,
                                                         BinaryData::right_footswitch_idle_pngSize),
                                                         juce::ImageCache::getFromMemory(BinaryData::right_footswitch_pressed_png,
                                                         BinaryData::right_footswitch_pressed_pngSize));

    for (const auto& footswitch : footswitches)
        addAndMakeVisible(*footswitch);

}
void Pedal::resizeFootswitches()
{
    //this should be relational with image size and wrt each other so that i can iterate through and set bounds
    footswitches[0]->setBounds(155, 420, 90, 90);
    footswitches[1]->setBounds(430, 420, 90, 90);
    footswitches[2]->setBounds(705, 420, 90, 90);
}
void Pedal::initializeKnobs()
{
    for (auto& knob : knobs)
    {
        if (&knob == &knobs.front()) // i need to use & because it compares the addresses. If i didnt use & it would compare the object themselves
            knob = std::make_unique<Knob>(juce::ImageCache::getFromMemory(BinaryData::left_side_knob_png,
                                            BinaryData::left_side_knob_pngSize));
        else
            knob = std::make_unique<Knob>(juce::ImageCache::getFromMemory(BinaryData::right_side_knob_png,
											BinaryData::right_side_knob_pngSize));
        
        knob.get()->setLookAndFeel(knob.get());
        knob->setRange(0.0f, 100.0f, 1.0f);
        knob->setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        knob->setTextBoxStyle(juce::Slider::NoTextBox, true, 90, 90);
        knob->setValue(0.0f);
        addAndMakeVisible(*knob);
    }		
   
}

void Pedal::resizeKnobs()
{
    //this should be relational with image size and wrt each other so that i can iterate through and set bounds
    knobs[0]->setBounds(155, 115, 90, 90);
    knobs[1]->setBounds(525, 50, 90, 90);
    knobs[2]->setBounds(625, 50, 90, 90);
    knobs[3]->setBounds(725, 50, 90, 90);
    knobs[4]->setBounds(625, 160, 90, 90);
    knobs[5]->setBounds(725, 160, 90, 90);
}


void Pedal::initializeComponents()
{
    initializeFootswitches();
    initializeKnobs();
}
void Pedal::resizeComponents()
{
    resizeFootswitches();
    resizeKnobs();
}

void Pedal::updateParamsString()
{
    paramsString.clear();
    paramsString.clear();
    auto& params = pedalAPVTS.state;
    for (int i = 0; i < params.getNumChildren(); ++i)
    {
        juce::ValueTree param = params.getChild(i);
        if (param.isValid())
        {
            juce::String paramID = param.getProperty("id").toString();
            auto* parameter = pedalAPVTS.getParameter(paramID);
            if (parameter != nullptr)
            {
                paramsString += parameter->getName(100) + ": " + parameter->getCurrentValueAsText() + "\n";
            }
        }
    }
}
