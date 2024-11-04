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
#include <iostream> 

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
	apvtsElemSize = pedalAPVTS.state.getNumChildren();
	DBG(apvtsElemSize);
}


Pedal::~Pedal()
{

}

void Pedal::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::lightgrey);
    g.drawImage(pedalBaseImage, pedalBounds.toFloat());


    if ( !isEditPage ) 
    {

        //PREVIOUSLY USED FOR SIMPLE EDIT PAGE
        g.drawRoundedRectangle(juce::Rectangle<float>(innerLeft, innerTop, innerWidth, innerHeight), cornerSize, 2.0f); //simple edit page
        g.setFont(customFontLookAndFeel.getCustomFont());
        g.setFont(18.5f);
        juce::String text = "VIBRATO";
        auto m = CustomFontLookAndFeel::getCustomFont().getStringWidth(text);
        g.drawText(text, innerLeft, innerTop + 1, (innerRight - innerLeft), 20, juce::Justification::horizontallyCentred);
        g.setFont(18.5f);
        juce::StringArray lines;
        lines.addLines(paramsString);
        int lineHeight = g.getCurrentFont().getHeight();
        for (int i = 0; i < 4; ++i)
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
        }
    }
    else
    {

        //FULL EDIT PAGE
        int scrollBarWidth = 5;
        int scrollBarHeight = 83;
        int scrollBarX = outerRight - 5;
        int scrollBarY = outerTop + 26;
        g.setColour(juce::Colours::white);
        g.drawRect(scrollBarX, scrollBarY, scrollBarWidth, scrollBarHeight);
        juce::StringArray lines;
        lines.addLines(paramsString);
        int totalItems = lines.size() - 1;
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

        g.drawLine(outerLeft, outerTop + 22, outerRight, outerTop + 22, 2.0f);
        auto ft = customFontLookAndFeel.getCustomFont().boldened();
        ft.setExtraKerningFactor(0.1f);
        g.setFont(ft);
        g.setFont(15.5f);
        juce::String text = "EDIT ";
        g.drawText(text, outerLeft + 2, outerTop + 2, 8, 20, juce::Justification::left);
        g.setFont(customFontLookAndFeel.getCustomFont());
        g.setFont(16.5f);
        juce::String text2 = "[VIBRATO] ";
        g.drawText(text2, outerLeft + 130, outerTop + 2, 20, 20, juce::Justification::left);
        g.setFont(18.0f);
        int lineHeight = g.getCurrentFont().getHeight();
        for (int i = 0; i < maxElemtoDisplay; ++i)
        {
            juce::String line = lines[i + displayOffset];
            int colonIndex = line.indexOfChar(':');
            juce::String paramName = line.substring(0, colonIndex + 1);
            juce::String paramValue = line.substring(colonIndex + 1).trim();
            if (displayOffset == 0)
            {
                if (colonIndex != -1)
                {

                    if (currentAPVTSIndex == i)
                    {
                        g.setColour(juce::Colours::lightgrey);
                        g.fillRect(outerLeft + 4, outerTop + 24 + i * lineHeight, (innerRight - 7 - innerLeft), lineHeight);

                        g.setColour(juce::Colours::black);
                        g.drawText(paramName, outerLeft + 4, outerTop + 24 + i * lineHeight, innerWidth, lineHeight, juce::Justification::centredLeft);
                        g.drawText(paramValue, innerRight - 35, outerTop + 24 + i * lineHeight, innerWidth, lineHeight, juce::Justification::centredLeft);
                    }
                    else
                    {
                        g.setColour(juce::Colours::white);
                        g.drawText(paramName, outerLeft + 4, outerTop + 24 + i * lineHeight, innerWidth, lineHeight, juce::Justification::centredLeft);
                        g.drawText(paramValue, innerRight - 35, outerTop + 24 + i * lineHeight, innerWidth, lineHeight, juce::Justification::centredLeft);
                    }

                }
            }
            else
            {
                if (currentAPVTSIndex - displayOffset == i) //index - offset
                {
                    g.setColour(juce::Colours::lightgrey);
                    g.fillRect(outerLeft + 4, outerTop + 24 + i * lineHeight, (innerRight - 7 - innerLeft), lineHeight);

                    g.setColour(juce::Colours::black);
                    g.drawText(paramName, outerLeft + 4, outerTop + 24 + i * lineHeight, innerWidth, lineHeight, juce::Justification::centredLeft);
                    g.drawText(paramValue, innerRight - 35, outerTop + 24 + i * lineHeight, innerWidth, lineHeight, juce::Justification::centredLeft);
                }
                else
                {
                    g.setColour(juce::Colours::white);
                    g.drawText(paramName, outerLeft + 4, outerTop + 24 + i * lineHeight, innerWidth, lineHeight, juce::Justification::centredLeft);
                    g.drawText(paramValue, innerRight - 35, outerTop + 24 + i * lineHeight, innerWidth, lineHeight, juce::Justification::centredLeft);
                }
            }

        }
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
        if (&knob == &knobs.front())
            { // i need to use & because it compares the addresses. If i didnt use & it would compare the object themselves
            knob = std::make_unique<Knob>(juce::ImageCache::getFromMemory(BinaryData::left_side_knob_png,
                                            BinaryData::left_side_knob_pngSize));
            knob->setRange(0.0f, 7.0f, 1.0f);
            }
        else
        {
            knob = std::make_unique<Knob>(juce::ImageCache::getFromMemory(BinaryData::right_side_knob_png,
                BinaryData::right_side_knob_pngSize));
            knob->setRange(0.0f, 100.0f, 1.0f);
        }
            
        
        knob.get()->setLookAndFeel(knob.get());
        knob->setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        knob->setTextBoxStyle(juce::Slider::NoTextBox, true, 90, 90);
        knob->setValue(0.0f);
        knob->addListener(this);
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
	initializeButtons();
}
void Pedal::resizeComponents()
{
    resizeFootswitches();
    resizeKnobs();
    resizeButtons();
}

void Pedal::updateParamsString()
{

    paramsString.clear();
    auto& params = pedalAPVTS.state;

    // Calculate the maximum width of parameter names
    int maxWidth = 0;
    for (int i = 0; i < params.getNumChildren(); ++i)
    {
        juce::ValueTree param = params.getChild(i);
        if (param.isValid())
        {
            juce::String paramID = param.getProperty("id").toString();
            auto* parameter = pedalAPVTS.getParameter(paramID);
            if (parameter != nullptr)
            {
                int width = parameter->getName(100).length();
                if (width > maxWidth)
                    maxWidth = width;
            }
        }
    }

    // Store parameter names and values
    for (int i = 0; i < params.getNumChildren(); ++i)
    {
        juce::ValueTree param = params.getChild(i);
        if (param.isValid())
        {
            juce::String paramID = param.getProperty("id").toString();
            auto* parameter = pedalAPVTS.getParameter(paramID);
            if (parameter != nullptr)
            {
                juce::String paramName = parameter->getName(100);
                juce::String paramValue = parameter->getCurrentValueAsText();
                paramsString += paramName + ": " + paramValue + "\n";
            }
        }
    }
}

void Pedal::initializeButtons()
{
    upButton.setButtonText("up");
    downButton.setButtonText("down");
    editButton.setButtonText("edit");
    exitButton.setButtonText("exit");

    downButton.onClick = [this]() { downButtonClicked(); };
    upButton.onClick = [this]() { upButtonClicked(); }; 
    editButton.onClick = [this]() { editButtonClicked(); };
    exitButton.onClick = [this]() { exitButtonClicked(); };

    addAndMakeVisible(upButton);
    addAndMakeVisible(downButton);
    addAndMakeVisible(editButton);
    addAndMakeVisible(exitButton);
}

void Pedal::resizeButtons()
{
    upButton.setBounds(525, 160, 50, 30); 
    downButton.setBounds(525, 200, 50, 30); 
    editButton.setBounds(525, 240, 50, 30); 
    exitButton.setBounds(525, 280, 50, 30); 
}

void Pedal::downButtonClicked()
{
    if (isEditPage)
    {
        DBG("Down");
        DBG("APVTS Index: ");  DBG(currentAPVTSIndex);
        if (currentAPVTSIndex < apvtsElemSize - 1)
        {
            currentAPVTSIndex += 1;
            if (currentAPVTSIndex < maxElemtoDisplay)
            {
                DBG(" New APVTS Index: ");  DBG(currentAPVTSIndex);
                //paint the indexed parameter
                repaint(outerLeft, outerTop, (outerRight - outerLeft), (outerBottom - outerTop));
            }
            else
            {
                if (maxElemtoDisplay + displayOffset < apvtsElemSize)
                    displayOffset += 1;
                DBG("New APVTS Index: ");  DBG(currentAPVTSIndex);
                DBG("displayOffset: ");  DBG(displayOffset);
                repaint(outerLeft, outerTop, (outerRight - outerLeft), (outerBottom - outerTop));
            }
        }
    }

}

void Pedal::upButtonClicked() 
{
    if (isEditPage)
    {
        DBG("Up");
        DBG("APVTS Index: ");  DBG(currentAPVTSIndex);
        if (currentAPVTSIndex > 0)
        {
            currentAPVTSIndex -= 1;
            if (currentAPVTSIndex >= displayOffset)
            {
                DBG(" New APVTS Index: ");  DBG(currentAPVTSIndex);
                //paint the indexed parameter
                repaint(outerLeft, outerTop, (outerRight - outerLeft), (outerBottom - outerTop));
            }
            else
            {
                if (displayOffset > 0)
                    displayOffset -= 1;
                DBG("New APVTS Index: ");  DBG(currentAPVTSIndex);
                DBG("displayOffset: ");  DBG(displayOffset);
                repaint(outerLeft, outerTop, (outerRight - outerLeft), (outerBottom - outerTop));
            }
        }
    }

}
void Pedal::editButtonClicked()
{
    isEditPage = true;
	repaint(outerLeft,outerTop,(outerRight-outerLeft), (outerBottom-outerTop));
}
void Pedal::exitButtonClicked()
{
	isEditPage = false; 
	displayOffset = 0;
	currentAPVTSIndex = 0;
    repaint(outerLeft, outerTop, (outerRight - outerLeft), (outerBottom - outerTop));
}

void Pedal::sliderValueChanged(juce::Slider* slider)
{
    if (knobs[0].get() == slider)
    {
        DBG("Knob 1");
        DBG(knobs[0].get()->getValue());
		set_EffectType(knobs[0].get()->getValue());
	}
	else if(knobs[1].get() == slider)
	{
		DBG("Knob 2");
	}
	else if(knobs[2].get() == slider)
	{
		DBG("Knob 3");
	}
	else if(knobs[3].get() == slider)
	{
		DBG("Knob 4");
	}
	else if(knobs[4].get() == slider)
	{
		DBG("Knob 5");
	}
    else if (knobs[5].get() == slider)
    {
        DBG("Knob 6");
    }
}

void Pedal::buttonClicked(juce::Button* button)
{

}

void Pedal::set_EffectType(float sliderVal)
{
	selectedEffect = static_cast<EffectType>(sliderVal);
}