/*
  ==============================================================================

    flanger.h
    Created: 14 Feb 2025 3:58:35pm
    Author:  mert.kabukcuoglu

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <cmath>

class Flanger
{
public:
    Flanger() = default;
    ~Flanger() = default;

    // Method to add flanger parameters to the APVTS layout.
    std::unique_ptr<juce::AudioProcessorParameterGroup> createFlangerParameterGroup();

    // Fetch parameter values from APVTS (via setters or a fetch method)
    void fetchParametersFromAPVTS(const juce::AudioProcessorValueTreeState& apvts);

    // Prepare the flanger engine for playback.
    void prepareToPlay(double sampleRate, int samplesPerBlock);

    // Process audio buffer using the flanger effect.
    void processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midiMessages, float sampleRate);

private:
    // Delay buffer for the flanger effect (circular buffer)
    juce::AudioBuffer<float> delayBuffer;
    int delayBufferSamples = 0;
    int delayBufferChannels = 0;
    int delayWritePosition = 0;

    // LFO state
    float lfoPhase = 0.0f;
    float inverseSampleRate = 1.0f;
    const float twoPi = juce::MathConstants<float>::twoPi;

    // Flanger engine parameters:
    // RATE: 0.01–20.00 Hz
    float rate = 1.0f;
    // DEPTH: 0–100 (modulation depth in milliseconds, for example)
    float depth = 50.0f;
    // FEEDBACK: 0–100 (amount of delayed signal fed back)
    float feedback = 50.0f;
    // WAVEFORM: 1–10, stored as 0-based index
    int waveform = 0;
    // INPUT SENS: 0–100 (affects sweep width)
    float inputSensitivity = 50.0f;
    // POLARITY: UP or DOWN (true for UP, false for DOWN)
    bool polarityUp = true;
    // OUTPUT MODE: MONO or STEREO (true for stereo, false for mono)
    bool outputStereo = true;
    // LOW DAMP: -100–0 (affects low-frequency feedback content)
    float lowDamp = -50.0f;
    // HIGH DAMP: -100–0 (affects high-frequency feedback content)
    float highDamp = -50.0f;
    // LOW CUT: FLAT or 20–800 Hz (FLAT is represented by -1)
    float lowCut = -1.0f;
    // HIGH CUT: FLAT or 630 Hz–16.0 kHz (FLAT is represented by -1)
    float highCut = -1.0f;
    // EFFECT LEVEL: 0–100 (wet mix)
    float effectLevel = 50.0f;
    float directLevel = 50.0f;

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

        float operator()(float phase, float depth, waveform wf) const
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
                out = phase < 0.25f ? 1.0f : 0.0f;
                break;
            case waveform::RampUp:
                out = phase < 0.5f ? phase * 2.0f : 2.0f * (phase - 0.5f);
                break;
            case waveform::RampDown:
                out = phase < 0.5f ? 1.0f - phase * 2.0f : 1.0f - 2.0f * (phase - 0.5f);
                break;
            case waveform::Random:
            {
                static float randomValue = static_cast<float>(rand()) / RAND_MAX;
                if (phase < 0.1f)
                    randomValue = static_cast<float>(rand()) / RAND_MAX;
                out = randomValue;
                break;
            }
            case waveform::Harmonic:
                out = 0.5f + 0.25f * std::sin(juce::MathConstants<float>::twoPi * phase)
                    + 0.25f * std::sin(2.0f * juce::MathConstants<float>::twoPi * phase);
                break;
            default:
                out = 0.0f;
                break;
            }
            return depth * out;
        }
    } itsLFO;
};