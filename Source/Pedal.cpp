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
    initializeComponents();

    parameterGroups["vibrato"] =
    {
        pedalAPVTS.getParameter("_Vibrato_A"),
        pedalAPVTS.getParameter("_Vibrato_B"),
        pedalAPVTS.getParameter("_Vibrato_C"),
        pedalAPVTS.getParameter("_Vibrato_D"),
        pedalAPVTS.getParameter("Vibrato_E"),
        pedalAPVTS.getParameter("Vibrato_F"),
        pedalAPVTS.getParameter("Vibrato_G"),
    };

    parameterGroups["flanger"] =
    {
        pedalAPVTS.getParameter("_Flanger_A"),
        pedalAPVTS.getParameter("_Flanger_B"),
        pedalAPVTS.getParameter("_Flanger_C"),
        pedalAPVTS.getParameter("_Flanger_D"),
        pedalAPVTS.getParameter("Flanger_E"),
        pedalAPVTS.getParameter("Flanger_F"),
        pedalAPVTS.getParameter("Flanger_G"),
    };


	effects = { {0,"vibrato"}, {1,"flanger"} }; //didnt like this approach but i will keep it for now
    selectedEffect = effects[0];
	selectedPage = pageType::BASIC_EDIT;
    currentOLEDPage = pageFactory.create(selectedPage, selectedEffect, parameterGroups[selectedEffect]);
	currentAPVTSIndex = -1;
    traverseAPVTSNodes();
}


Pedal::~Pedal()
{

}

void Pedal::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::lightgrey);
    g.drawImage(pedalBaseImage, pedalBounds.toFloat());
    currentOLEDPage->set_currentAPVTSIndex(currentAPVTSIndex);
    currentOLEDPage->paint(g);
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
            knob->setRange(0.0f, 1.0f, 1.0f);
            }
        else
        {
            knob = std::make_unique<Knob>(juce::ImageCache::getFromMemory(BinaryData::right_side_knob_png,
                BinaryData::right_side_knob_pngSize));
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
    if (selectedPage == pageType::FULL_EDIT)
    {
        if ( currentAPVTSIndex < parameterGroups[selectedEffect].size() - 1 )
        {
            currentOLEDPage->set_currentAPVTSIndex( ++currentAPVTSIndex );
            if (currentAPVTSIndex < maxElemtoDisplay)
                repaint(outerLeft, outerTop, (outerRight - outerLeft), (outerBottom - outerTop));
            else
            {
                if ( maxElemtoDisplay + displayOffset < parameterGroups[selectedEffect].size() )
                   currentOLEDPage->set_displayOffset( ++displayOffset );
                repaint(outerLeft, outerTop, (outerRight - outerLeft), (outerBottom - outerTop));
            }
        }
    }
}

void Pedal::upButtonClicked() 
{
    if (selectedPage == pageType::FULL_EDIT)
    {
        if (currentAPVTSIndex > 0)
        {
            currentOLEDPage->set_currentAPVTSIndex(--currentAPVTSIndex);
            if (currentAPVTSIndex >= displayOffset)
                repaint(outerLeft, outerTop, (outerRight - outerLeft), (outerBottom - outerTop));
            else
            {
                if (displayOffset > 0)
                    currentOLEDPage->set_displayOffset(--displayOffset);
                repaint(outerLeft, outerTop, (outerRight - outerLeft), (outerBottom - outerTop));
            }
        }
    }

}
void Pedal::editButtonClicked()
{
    if (selectedPage != pageType::FULL_EDIT)
    {
	   selectedPage = pageType::FULL_EDIT;
       displayOffset = 0;
       currentAPVTSIndex = 0;
       currentOLEDPage = pageFactory.create(selectedPage, selectedEffect, parameterGroups[selectedEffect]);
       repaint(outerLeft, outerTop, (outerRight - outerLeft), (outerBottom - outerTop));
    }


}
void Pedal::exitButtonClicked()
{
    selectedPage = pageType::BASIC_EDIT;
	displayOffset = 0;
	currentAPVTSIndex = -1;
    currentOLEDPage = pageFactory.create(selectedPage, selectedEffect, parameterGroups[selectedEffect]);
    repaint(outerLeft, outerTop, (outerRight - outerLeft), (outerBottom - outerTop));
}

void Pedal::sliderValueChanged(juce::Slider* slider)
{
    if (knobs[0].get() == slider)
    {
		selectedEffect = effects[static_cast<int>(knobs[0].get()->getValue())];
        if (selectedPage == pageType::FULL_EDIT)
        {
            currentAPVTSIndex = 0;
            currentOLEDPage.get()->set_currentAPVTSIndex(currentAPVTSIndex);
            currentOLEDPage = pageFactory.create(selectedPage, selectedEffect, parameterGroups[selectedEffect]);
        }
        else
        {
            currentAPVTSIndex = -1;
            currentOLEDPage.get()->set_currentAPVTSIndex(currentAPVTSIndex);
            currentOLEDPage = pageFactory.create(selectedPage, selectedEffect, parameterGroups[selectedEffect]);
        }
        displayOffset = 0;
    }
    repaint(outerLeft, outerTop, (outerRight - outerLeft), (outerBottom - outerTop));
}
void Pedal::sliderDragStarted(juce::Slider* slider)
{
	auto* knob = dynamic_cast<Knob*>(slider);

    if ( 1 == knob->getIndex() )
    {
        if(selectedPage == pageType::FULL_EDIT)
            knob->itsAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(pedalAPVTS,
                parameterGroups[selectedEffect][currentAPVTSIndex]->getParameterID(), *knob);
        else
            knob->itsAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(pedalAPVTS,
                parameterGroups[selectedEffect][4]->getParameterID(), *knob);
    }
    else if( 0 == knob->getIndex() )
		return;
    else
    {
        auto apvtsIndex = knob->getIndex() - 2;
		knob->itsAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(pedalAPVTS,
		    parameterGroups[selectedEffect][apvtsIndex]->getParameterID(), *knob);
        
		if (selectedPage != pageType::FULL_EDIT)
			currentAPVTSIndex = apvtsIndex;
    }
    repaint(outerLeft, outerTop, (outerRight - outerLeft), (outerBottom - outerTop));
}
 


void Pedal::sliderDragEnded(juce::Slider* slider)
{
    dynamic_cast<Knob*>(slider)->detachFromAPVTS();
}


void Pedal::buttonClicked(juce::Button* button)
{

}

void Pedal::traverseAPVTSNodes()
{
    auto root = pedalAPVTS.state; // Get the root ValueTree

    std::function<void(const juce::ValueTree&, int)> traverse = [&](const juce::ValueTree& tree, int depth)
        {
            // Print the current node's type and properties
            DBG(std::string(depth * 2, ' ') << "Node: " << tree.getType().toString());
            for (int i = 0; i < tree.getNumProperties(); ++i)
            {
                auto propertyName = tree.getPropertyName(i).toString();
                auto propertyValue = tree.getProperty(tree.getPropertyName(i)).toString();
                DBG(std::string(depth * 2 + 2, ' ') << propertyName << ": " << propertyValue);
            }

            // Recursively traverse the children
            for (int i = 0; i < tree.getNumChildren(); ++i)
            {
                traverse(tree.getChild(i), depth + 1);
            }
        };

    traverse(root, 0);
}