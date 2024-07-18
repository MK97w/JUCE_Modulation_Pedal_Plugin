/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Modulation_Pedal_PluginAudioProcessorEditor::Modulation_Pedal_PluginAudioProcessorEditor(Modulation_Pedal_PluginAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    DBG("check 1");
	/*setSize(modulationPedal.getBounds().getWidth(),
            modulationPedal.getBounds().getHeight()); //Set size will be called after the pedal is created and given thw arguments of png size*/
   
 /*   footswitch_test[0] = std::make_unique<FootswitchButton>(juce::ImageCache::getFromMemory(BinaryData::left_footswitch_idle_png, BinaryData::left_footswitch_idle_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::left_footswitch_pressed_png, BinaryData::left_footswitch_idle_pngSize));

    footswitch_test[1] = std::make_unique<FootswitchButton>(juce::ImageCache::getFromMemory(BinaryData::mid_footswitch_idle_png, BinaryData::mid_footswitch_idle_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::mid_footswitch_pressed_png, BinaryData::mid_footswitch_pressed_pngSize));

    footswitch_test[2] = std::make_unique<FootswitchButton>(juce::ImageCache::getFromMemory(BinaryData::right_footswitch_idle_png, BinaryData::right_footswitch_idle_pngSize),
        juce::ImageCache::getFromMemory(BinaryData::right_footswitch_pressed_png, BinaryData::right_footswitch_pressed_pngSize));

    for (const auto& footswitch : footswitch_test)
        addAndMakeVisible(*footswitch);*/

    //setSize(800, 600);
    setSize(modulationPedal.getBounds().getWidth(),
        modulationPedal.getBounds().getHeight());

}

Modulation_Pedal_PluginAudioProcessorEditor::~Modulation_Pedal_PluginAudioProcessorEditor()
{

}

//==============================================================================
void Modulation_Pedal_PluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    DBG("check 2");
    //modulationPedal.paint(g);
}

void Modulation_Pedal_PluginAudioProcessorEditor::resized()
{
    DBG("check 3");
    modulationPedal.resized();
    /*footswitch_test[0]->setBounds(155, 420, 90, 90); //footswitches should return the image sizes and i should call them here
    footswitch_test[1]->setBounds(430, 420, 90, 90);
    footswitch_test[2]->setBounds(705, 420, 90, 90);

    footswitch_test[0]->redraw();
    footswitch_test[1]->redraw();
    footswitch_test[2]->redraw();*/
}

