/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Modulation_Pedal_PluginAudioProcessorEditor::Modulation_Pedal_PluginAudioProcessorEditor(Modulation_Pedal_PluginAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p) ,modulationPedal(audioProcessor)
{ 
    setSize(modulationPedal.getBounds().getWidth(),
        modulationPedal.getBounds().getHeight());

    addAndMakeVisible(modulationPedal);
    
}

Modulation_Pedal_PluginAudioProcessorEditor::~Modulation_Pedal_PluginAudioProcessorEditor()
{
	setLookAndFeel(nullptr);
}

//==============================================================================
void Modulation_Pedal_PluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    modulationPedal.paint(g);
}

void Modulation_Pedal_PluginAudioProcessorEditor::resized()
{
    modulationPedal.setBounds(getLocalBounds());
}