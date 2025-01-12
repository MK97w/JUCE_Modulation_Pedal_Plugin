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
            Square,
            Pulse,
            RampUp,
            RampDown,
            Random,
            Harmonic
        };
        
        
        float operator()(float phase, float depth, waveform wf)
		{
            float out = 0.0f;

            switch (wf) 
            {
            case waveform::Sine:
                out = 0.5f + 0.5f * std::sin(juce::MathConstants<float>::twoPi * phase);
                break;

            case waveform::Triangle:
                out = phase < 0.5f ? 2.0f * phase : 2.0f * (1.0f - phase);
                break;

            case waveform::Sawtooth:
                out = phase;
                break;

            case waveform::InverseSawtooth:
                out = 1.0f - phase;
                break;

            case waveform::Square:
                out = phase < 0.5f ? 1.0f : 0.0f;
                break;

            case waveform::Pulse:
                out = phase < 0.25f ? 1.0f : 0.0f; // Adjustable duty cycle could be parameterized.
                break;

            case waveform::RampUp:
                out = phase < 0.5f ? phase * 2.0f : 2.0f * (phase - 0.5f);
                break;

            case waveform::RampDown:
                out = phase < 0.5f ? 1.0f - phase * 2.0f : 1.0f - 2.0f * (phase - 0.5f);
                break;

            case waveform::Random:
                // Generate random output for each phase reset
                static float randomValue = static_cast<float>(rand()) / RAND_MAX;
                if (phase < 0.1f) // Update random value near phase reset
                    randomValue = static_cast<float>(rand()) / RAND_MAX;
                out = randomValue;
                break;

            case waveform::Harmonic:
                out = 0.5f + 0.25f * std::sin(juce::MathConstants<float>::twoPi * phase)
                    + 0.25f * std::sin(2.0f * juce::MathConstants<float>::twoPi * phase);
                break;

            default:
                out = 0.0f; // Fallback for invalid waveform
                break;
            }
            return depth * out;
		}
    };
};
