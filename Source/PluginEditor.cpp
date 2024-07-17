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
	setSize(modulationPedal.getBounds().getWidth(), 
            modulationPedal.getBounds().getHeight()); //Set size will be called after the pedal is created and given thw arguments of png size
}

Modulation_Pedal_PluginAudioProcessorEditor::~Modulation_Pedal_PluginAudioProcessorEditor()
{

}

//==============================================================================
void Modulation_Pedal_PluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    modulationPedal.paint(g);
}

void Modulation_Pedal_PluginAudioProcessorEditor::resized()
{
    modulationPedal.resized();
}

