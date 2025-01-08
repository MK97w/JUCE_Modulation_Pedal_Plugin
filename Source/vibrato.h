/*
  ==============================================================================

    vibrato.h
    Created: 10 Dec 2024 5:56:28pm
    Author:  mert.kabukcuoglu

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>	

//using twoPi = juce::MathConstants<double>::twoPi;


class Vibrato
{
public:

    std::unique_ptr<juce::AudioProcessorParameterGroup> createVibratoParameterGroup();
    void prepareToPlay(double sampleRate, int samplesPerBlock);
	void processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midiMessages);

private:
	juce::AudioBuffer<float> delayBuffer;
    int delayBufferSamples;
    int delayBufferChannels;
    int delayWritePosition;

    float lfoPhase;
    float inverseSampleRate;
    float twoPi;
    
    struct lfo
    {
        enum class waveform
        {
            Sine,
            Triangle,
            Sawtooth,
            InverseSawtooth,
            Square
        };
        
        
        float operator()(float phase, float depth, waveform wf)
		{
            float out = 0.0f;

            switch (wf) 
            {
                case waveform::Sine : 
                {
                    out = 0.5f + 0.5f * sinf(juce::MathConstants<double>::twoPi * phase);
                    break;
                }
                case waveform::Triangle: 
                {
                    if (phase < 0.25f)
                        out = 0.5f + 2.0f * phase;
                    else if (phase < 0.75f)
                        out = 1.0f - 2.0f * (phase - 0.25f);
                    else
                        out = 2.0f * (phase - 0.75f);
                    break;
                }
                case  waveform::Sawtooth:
                {
                    if (phase < 0.5f)
                        out = 0.5f + phase;
                    else
                        out = phase - 0.5f;
                    break;
                }
                case waveform::InverseSawtooth:   
                {
                    if (phase < 0.5f)
                        out = 0.5f - phase;
                    else
                         out = 1.5f - phase;
                    break;
                }
            }
            return depth * out;
		}
    };
};
