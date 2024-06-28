/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Modulation_Pedal_PluginAudioProcessorEditor::Modulation_Pedal_PluginAudioProcessorEditor(Modulation_Pedal_PluginAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p), 
    leftKnob(juce::ImageCache::getFromMemory(BinaryData::left_side_knob_png, BinaryData::left_side_knob_pngSize)),
    rightKnob_1(juce::ImageCache::getFromMemory(BinaryData::right_side_knob_png, BinaryData::right_side_knob_pngSize)),
    rightKnob_2(juce::ImageCache::getFromMemory(BinaryData::right_side_knob_png, BinaryData::right_side_knob_pngSize)),
    rightKnob_3(juce::ImageCache::getFromMemory(BinaryData::right_side_knob_png, BinaryData::right_side_knob_pngSize)),
    rightKnob_4(juce::ImageCache::getFromMemory(BinaryData::right_side_knob_png, BinaryData::right_side_knob_pngSize)),
    rightKnob_5(juce::ImageCache::getFromMemory(BinaryData::right_side_knob_png, BinaryData::right_side_knob_pngSize)),
 

    leftFootswitch(juce::ImageCache::getFromMemory(BinaryData::left_footswitch_idle_png, BinaryData::left_footswitch_idle_pngSize),
                   juce::ImageCache::getFromMemory(BinaryData::left_footswitch_pressed_png, BinaryData::left_footswitch_idle_pngSize)),
    
    midFootswitch(juce::ImageCache::getFromMemory(BinaryData::mid_footswitch_idle_png, BinaryData::mid_footswitch_idle_pngSize),
                  juce::ImageCache::getFromMemory(BinaryData::mid_footswitch_pressed_png, BinaryData::mid_footswitch_pressed_pngSize)),
    
    rightFootswitch(juce::ImageCache::getFromMemory(BinaryData::right_footswitch_idle_png, BinaryData::right_footswitch_idle_pngSize),
                   juce::ImageCache::getFromMemory(BinaryData::right_footswitch_pressed_png, BinaryData::right_footswitch_pressed_pngSize))
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (960, 540);

    leftKnob.setLookAndFeel(&leftKnob);
    //leftKnob.setRange(0.0f, 20.0f,5.0f); temporarly disabled step size.
    leftKnob.setRange(-60.0f, 0.0f, 0.01f);
    leftKnob.setValue(-20.0f);
    leftKnob.addListener(this);
    addAndMakeVisible(leftKnob);
    leftKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    leftKnob.setTextBoxStyle(juce::Slider::NoTextBox, true, 90, 90);

    rightKnob_1.setLookAndFeel(&rightKnob_1);
    rightKnob_1.setRange(0.0f, 10.0f);
    addAndMakeVisible(rightKnob_1);
    rightKnob_1.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    rightKnob_1.setTextBoxStyle(juce::Slider::NoTextBox, true, 90, 90);

    rightKnob_2.setLookAndFeel(&rightKnob_2);
    rightKnob_2.setRange(0.0f, 10.0f);
    addAndMakeVisible(rightKnob_2);
    rightKnob_2.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    rightKnob_2.setTextBoxStyle(juce::Slider::NoTextBox, true, 90, 90);

    rightKnob_3.setLookAndFeel(&rightKnob_3);
    rightKnob_3.setRange(0.0f, 10.0f);
    addAndMakeVisible(rightKnob_3);
    rightKnob_3.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    rightKnob_3.setTextBoxStyle(juce::Slider::NoTextBox, true, 90, 90);

    rightKnob_4.setLookAndFeel(&rightKnob_4);
    rightKnob_4.setRange(0.0f, 10.0f);
    addAndMakeVisible(rightKnob_4);
    rightKnob_4.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    rightKnob_4.setTextBoxStyle(juce::Slider::NoTextBox, true, 90, 90);

    rightKnob_5.setLookAndFeel(&rightKnob_5);
    rightKnob_5.setRange(0.0f, 10.0f);
    addAndMakeVisible(rightKnob_5);
    rightKnob_5.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    rightKnob_5.setTextBoxStyle(juce::Slider::NoTextBox, true, 90, 90);

    addAndMakeVisible(leftFootswitch);
    addAndMakeVisible(midFootswitch);
    addAndMakeVisible(rightFootswitch);


    rate.setText("RATE/VALUE", juce::dontSendNotification);
    rate.setFont(juce::Font(16.0f,juce::Font::bold));
    rate.setColour(juce::Label::textColourId, juce::Colours::black);
    rate.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(rate);


    depth.setText("DEPTH", juce::dontSendNotification);
    depth.setFont(juce::Font(16.0f, juce::Font::bold));
    depth.setColour(juce::Label::textColourId, juce::Colours::black);
    depth.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(depth);


    level.setText("LEVEL", juce::dontSendNotification);
    level.setFont(juce::Font(16.0f, juce::Font::bold));
    level.setColour(juce::Label::textColourId, juce::Colours::black);
    level.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(level);

    param1.setText("PARAM 1", juce::dontSendNotification);
    param1.setFont(juce::Font(16.0f, juce::Font::bold));
    param1.setColour(juce::Label::textColourId, juce::Colours::black);
    param1.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(param1);

    param2.setText("PARAM 2", juce::dontSendNotification);
    param2.setFont(juce::Font(16.0f, juce::Font::bold));
    param2.setColour(juce::Label::textColourId, juce::Colours::black);
    param2.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(param2);



}

Modulation_Pedal_PluginAudioProcessorEditor::~Modulation_Pedal_PluginAudioProcessorEditor()
{
    leftKnob.setLookAndFeel(nullptr);
    rightKnob_1.setLookAndFeel(nullptr);
    rightKnob_2.setLookAndFeel(nullptr);
    rightKnob_3.setLookAndFeel(nullptr);
    rightKnob_4.setLookAndFeel(nullptr);
    rightKnob_5.setLookAndFeel(nullptr);
}

//==============================================================================
void Modulation_Pedal_PluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.drawImage(juce::ImageCache::getFromMemory(BinaryData::pedal_base_png, BinaryData::pedal_base_pngSize), getLocalBounds().toFloat());
    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
}

void Modulation_Pedal_PluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    leftKnob.setBounds(155,115,90,90);
    rightKnob_1.setBounds(525, 50, 90, 90);
    rightKnob_2.setBounds(625, 50, 90, 90);
    rightKnob_3.setBounds(725, 50, 90, 90);
    rightKnob_4.setBounds(625, 160, 90, 90);
    rightKnob_5.setBounds(725, 160, 90, 90);

   /* rate.setBounds(525, 0, 90, 90);
    depth.setBounds(625, 0, 90, 90);
    level.setBounds(725, 0, 90, 90);
    param1.setBounds(625, 110, 90, 90);
    param2.setBounds(725, 110, 90, 90);
*/

    leftFootswitch.setBounds(155,420,90,90);
    midFootswitch.setBounds(430, 420, 90, 90);
    rightFootswitch.setBounds(705, 420, 90, 90);
}


void Modulation_Pedal_PluginAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &leftKnob)
    {
        audioProcessor.mGain = leftKnob.getValue();
    }
        
}