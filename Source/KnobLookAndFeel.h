/*
  ==============================================================================

    KnobLookAndFeel.h
    Created: 6 Jul 2024 12:17:42am
    Author:  MK

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Knob : public juce::Slider, public juce::LookAndFeel_V4
{
public:
    Knob(juce::Image sourceImage);
    void drawRotarySlider(juce::Graphics& g,
        int x,
        int y,
        int width,
        int height,
        float sliderPosProportional,
        float rotaryStartAngle,
        float rotaryEndAngle,
        juce::Slider& slider);

    ~Knob() { setLookAndFeel(nullptr); };
    void detachFromAPVTS();
	
    static inline int elementCount{0};
    int getIndex() { return index; };
    juce::Image img;
    std::unique_ptr < juce::AudioProcessorValueTreeState::SliderAttachment> itsAttachment;

private:
	int index;
 };