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
	setSize(960, 540); //Set size will be called after the pedal is created and given thw arguments of png size
}

Modulation_Pedal_PluginAudioProcessorEditor::~Modulation_Pedal_PluginAudioProcessorEditor()
{

}

//==============================================================================
void Modulation_Pedal_PluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(juce::FontOptions(15.0f));
    g.drawFittedText("Time to clean up the code!", getLocalBounds(), juce::Justification::centred, 1);
}

void Modulation_Pedal_PluginAudioProcessorEditor::resized()
{

}

