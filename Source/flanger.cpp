/*
  ==============================================================================

    flanger.cpp
    Created: 14 Feb 2025 3:58:35pm
    Author:  mert.kabukcuoglu

  ==============================================================================
*/

#include "flanger.h"


std::unique_ptr<juce::AudioProcessorParameterGroup> Flanger::createFlangerParameterGroup()
{
    auto flangerGroup = std::make_unique<juce::AudioProcessorParameterGroup>("flanger", "Flanger", "|");

    flangerGroup->addChild(std::make_unique<juce::AudioParameterFloat>("Flanger_Rate5", "Rate",
        juce::NormalisableRange<float>(0.01f, 20.0f, 0.01f), 1.0f));
    flangerGroup->addChild(std::make_unique<juce::AudioParameterFloat>("_Flanger_Depth1", "Depth",
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 50.0f));
    flangerGroup->addChild(std::make_unique<juce::AudioParameterFloat>("_Flanger_Feedback4", "Feedback",
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 50.0f));
    flangerGroup->addChild(std::make_unique<juce::AudioParameterChoice>("Flanger_Waveform3", "Waveform",
        juce::StringArray{ "Sine", "Triangle", "Sawtooth", "InverseSawtooth", "Square", "Pulse", "RampUp", "RampDown", "Random", "Harmonic" }, 0));
    flangerGroup->addChild(std::make_unique<juce::AudioParameterFloat>("Flanger_InputSens", "Input Sensitivity",
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 50.0f));
    flangerGroup->addChild(std::make_unique<juce::AudioParameterChoice>("Flanger_Polarity6", "Polarity",
        juce::StringArray{ "UP", "DOWN" }, 0));
    flangerGroup->addChild(std::make_unique<juce::AudioParameterChoice>("Flanger_OutputMode7", "Output Mode",
        juce::StringArray{ "MONO", "STEREO" }, 1));
    flangerGroup->addChild(std::make_unique<juce::AudioParameterFloat>("Flanger_LowDamp8", "Low Damp",
        juce::NormalisableRange<float>(-100.0f, 0.0f, 1.0f), -50.0f));
    flangerGroup->addChild(std::make_unique<juce::AudioParameterFloat>("Flanger_HighDamp9", "High Damp",
        juce::NormalisableRange<float>(-100.0f, 0.0f, 1.0f), -50.0f));
    flangerGroup->addChild(std::make_unique<juce::AudioParameterFloat>("Flanger_LowCut10", "Low Cut",
        juce::NormalisableRange<float>(-1.0f, 800.0f, 1.0f), -1.0f));  // -1 means FLAT
    flangerGroup->addChild(std::make_unique<juce::AudioParameterFloat>("Flanger_HighCut11", "High Cut",
        juce::NormalisableRange<float>(630.0f, 16000.0f, 1.0f), -1.0f)); // -1 means FLAT
    flangerGroup->addChild(std::make_unique<juce::AudioParameterFloat>("_Flanger_EffectLevel2", "Effect Level",
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 50.0f));
    flangerGroup->addChild(std::make_unique<juce::AudioParameterFloat>("_Flanger_DirectLevel3", "Direct Level",
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 50.0f));

    return flangerGroup;
}

void Flanger::fetchParametersFromAPVTS(const juce::AudioProcessorValueTreeState& apvts)
{
    rate = apvts.getRawParameterValue("Flanger_Rate")->load();
    depth = apvts.getRawParameterValue("Flanger_Depth")->load();
    feedback = apvts.getRawParameterValue("Flanger_Feedback")->load();
    waveform = static_cast<int>(apvts.getRawParameterValue("Flanger_Waveform")->load());
    inputSensitivity = apvts.getRawParameterValue("Flanger_InputSens")->load();
    polarityUp = (static_cast<int>(apvts.getRawParameterValue("Flanger_Polarity")->load()) == 0); // UP = 0
    outputStereo = (static_cast<int>(apvts.getRawParameterValue("Flanger_OutputMode")->load()) == 1); // STEREO = 1
    lowDamp = apvts.getRawParameterValue("Flanger_LowDamp")->load();
    highDamp = apvts.getRawParameterValue("Flanger_HighDamp")->load();
    lowCut = apvts.getRawParameterValue("Flanger_LowCut")->load();
    highCut = apvts.getRawParameterValue("Flanger_HighCut")->load();
    effectLevel = apvts.getRawParameterValue("Flanger_EffectLevel")->load();
    effectLevel = apvts.getRawParameterValue("Flanger_DirectLevel")->load();
}

void Flanger::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    inverseSampleRate = 1.0f / static_cast<float>(sampleRate);
    // For a flanger, a short delay buffer is typical, e.g., 20ms maximum delay.
    // Here, we choose a buffer size large enough to cover our max delay time.
    delayBufferSamples = static_cast<int>(sampleRate * 0.02f) + samplesPerBlock;
    delayBufferChannels = 2; // Typically stereo, can be adjusted based on output mode.
    delayBuffer.setSize(delayBufferChannels, delayBufferSamples);
    delayBuffer.clear();
    delayWritePosition = 0;
}

void Flanger::processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midiMessages, float sampleRate)
{
    // Update inverse sample rate
    inverseSampleRate = 1.0f / sampleRate;

    // For each channel, process the audio.
    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();

    // For each sample, we use our LFO to modulate the delay time.
    for (int channel = 0; channel < numChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer(channel);

        for (int sample = 0; sample < numSamples; ++sample)
        {
            // Increment LFO phase
            lfoPhase += rate * inverseSampleRate;
            if (lfoPhase >= 1.0f)
                lfoPhase -= 1.0f;

            // Incorporate initial phase offset (for flanger, you might want this too)
            float effectivePhase = std::fmod(lfoPhase + 0.0f, 1.0f); // If you want initialPhase, add it here normalized.

            // Compute base LFO output using our lfo functor.
            float lfoValueBase = itsLFO(effectivePhase, depth / 100.0f, static_cast<lfo::waveform>(waveform));
            // Optionally, blend with harmonic variant if complexity > 1.
            float lfoValue = lfoValueBase; // For simplicity, we'll use lfoValueBase.

            // Adjust modulation range based on input sensitivity.
            // For flanger, input sensitivity can expand the sweep: 
            // If polarity is DOWN, invert the modulation.
            // We read the current sample amplitude and scale modulation accordingly.
            float inputGain = std::abs(channelData[sample]);
            float adjustedModulation = (depth / 100.0f) * (1.0f + inputSensitivity * inputGain / 100.0f);
            if (!polarityUp)
                adjustedModulation = -adjustedModulation;

            // Compute the current delay in samples for modulation.
            // For a flanger, typical modulated delay times are in the range of 0 to, say, 10ms.
            float maxDelayInSamples = sampleRate * 0.01f; // 10ms
            int modulatedDelaySamples = static_cast<int>(lfoValue * adjustedModulation * maxDelayInSamples);

            // Write current dry sample (possibly scaled by direct level) into delay buffer.
            delayBuffer.setSample(channel, delayWritePosition, (channelData[sample] * directLevel));

            // Calculate read position for delay buffer (with wrap-around)
            int readPosition = delayWritePosition - modulatedDelaySamples;
            if (readPosition < 0)
                readPosition += delayBufferSamples;

            // Read delayed sample (simple linear interpolation could be added for smoother effect)
            float delayedSample = delayBuffer.getSample(channel, readPosition);

            // Apply feedback processing.
            // Here, we scale the delayed signal by feedback, then add it to the delay buffer.
            float fbSample = delayedSample * (feedback / 100.0f);

            // For simplicity, we omit complex filtering here. In a more advanced implementation,
            // you would process fbSample through low damp, high damp, low cut, and high cut filters.
            // For example, you might use one-pole filters or biquad filters.

            // Write the feedback sample back into the delay buffer at the write position (or mix it in).
            // For now, we simply add it to the delayed sample.
            delayedSample += fbSample;

            // Mix the wet signal (delayedSample scaled by effect level) with the dry signal.
            float outputSample = (channelData[sample] * directLevel) + (delayedSample * (effectLevel / 100.0f));
            channelData[sample] = outputSample;

            // Increment write position in delay buffer (circular)
            ++delayWritePosition;
            if (delayWritePosition >= delayBufferSamples)
                delayWritePosition = 0;
        }
    }

    // If output mode is MONO, sum the channels. For STEREO, leave them as is.
    if (!outputStereo && numChannels > 1)
    {
        for (int sample = 0; sample < numSamples; ++sample)
        {
            float monoSample = 0.0f;
            for (int channel = 0; channel < numChannels; ++channel)
                monoSample += buffer.getSample(channel, sample);
            monoSample /= static_cast<float>(numChannels);
            for (int channel = 0; channel < numChannels; ++channel)
                buffer.setSample(channel, sample, monoSample);
        }
    }
}

