/*
  ==============================================================================

    vibrato.h
    Created: 10 Dec 2024 5:56:28pm
    Author:  mert.kabukcuoglu

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>	


class Vibrato
{
public:

    std::unique_ptr<juce::AudioProcessorParameterGroup> createVibratoParameterGroup();
    void prepareToPlay(double sampleRate, int samplesPerBlock);
	void processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midiMessages);

private:
	juce::AudioBuffer<float> delayBuffer;
    struct lfo
    {
		float operator()(float phase, int waveform)
		{

		}
    };
};
