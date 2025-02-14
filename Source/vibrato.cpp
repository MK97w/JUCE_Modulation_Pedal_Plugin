#include "vibrato.h"


std::unique_ptr<juce::AudioProcessorParameterGroup> Vibrato::createVibratoParameterGroup()
{
    auto params = std::make_unique<juce::AudioProcessorParameterGroup>("vibratoGroup", "Vibrato", "|");

    params->addChild(std::make_unique<juce::AudioParameterFloat>("_Vibrato_Intensity1", "INTENSITY",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    params->addChild(std::make_unique<juce::AudioParameterFloat>("_Vibrato_Elevel2", "EFFECT LEVEL",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    params->addChild(std::make_unique<juce::AudioParameterFloat>("_Vibrato_Dlevel3", "DIRECT LEVEL",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    params->addChild(std::make_unique<juce::AudioParameterFloat>("_Vibrato_Depth4", "DEPTH",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    params->addChild(std::make_unique<juce::AudioParameterChoice>("Vibrato_WF5", "WAVEFORM",
        juce::StringArray{ "SIN", "TRI", "SWT", "SWTI", "SQR", "PLS", "RMPU", "RMPD", "RND", "HRM" }, 0));
    params->addChild(std::make_unique<juce::AudioParameterFloat>("Vibrato_Rate6", "RATE",
        juce::NormalisableRange<float>(0.1f, 10.0f, 0.01f), 1.0f));
    params->addChild(std::make_unique<juce::AudioParameterInt>("Vibrato_Complexity7", "COMPLEXITY", 1, 10, 5));
    params->addChild(std::make_unique<juce::AudioParameterFloat>("Vibrato_InputSensitivity8", "INPUT SENSITIVITY",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    params->addChild(std::make_unique<juce::AudioParameterFloat>("Vibrato_InitPhase9", "INITIAL PHASE",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f));

    return params;
}

void Vibrato::fetchParametersFromAPVTS(const juce::AudioProcessorValueTreeState& apvts)
{
    intensity = apvts.getRawParameterValue("_Vibrato_Intensity1")->load();
    effectLevel = apvts.getRawParameterValue("_Vibrato_Elevel2")->load();
    directLevel = apvts.getRawParameterValue("_Vibrato_Dlevel3")->load();
    depth = apvts.getRawParameterValue("_Vibrato_Depth4")->load();
    waveform = static_cast<int>(apvts.getRawParameterValue("Vibrato_WF5")->load());
    rate = apvts.getRawParameterValue("Vibrato_Rate6")->load();
    complexity = static_cast<int>(apvts.getRawParameterValue("Vibrato_Complexity7")->load());
    inputSensitivity = apvts.getRawParameterValue("Vibrato_InputSensitivity8")->load();
    initialPhase = apvts.getRawParameterValue("Vibrato_InitPhase9")->load();
    
}

void Vibrato::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    inverseSampleRate = 1.0f / static_cast<float>(sampleRate);
    delayBufferSamples = 2 * samplesPerBlock; // Allow up to 2x buffer size for delay
    delayBufferChannels = 2; // Stereo
    delayBuffer.setSize(delayBufferChannels, delayBufferSamples);
    delayBuffer.clear();
    delayWritePosition = 0;
}

void Vibrato::processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midiMessages, float sampleRate)
{
    // Update inverse sample rate and apply initial phase offset conversion
    inverseSampleRate = 1.0f / sampleRate;

    // Process each sample in each channel.
    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();

    // For each sample, compute an LFO value based on current phase and parameters.
    for (int channel = 0; channel < numChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer(channel);

        for (int sample = 0; sample < numSamples; ++sample)
        {
            // Update the LFO phase based on the rate. 
            lfoPhase += rate * inverseSampleRate;
            if (lfoPhase >= 1.0f)
                lfoPhase -= 1.0f;

            // Incorporate initial phase offset (assuming initialPhase is normalized [0,1])
            float effectivePhase = std::fmod(lfoPhase + initialPhase, 1.0f);

            // Compute base LFO output for the selected waveform
            float lfoValueBase = itsLFO(effectivePhase, depth, static_cast<lfo::waveform>(waveform));

            // Blend with a harmonic version if complexity > 1.
            float lfoValue = lfoValueBase;
            if (complexity > 1)
            {
                // Compute the harmonic version
                float lfoValueHarmonic = itsLFO(effectivePhase, depth, lfo::waveform::Harmonic);
                // Blend: map complexity from [1, 10] to [0, 1]
                float blend = (complexity - 1) / 9.0f;
                lfoValue = juce::jmap(blend, lfoValueBase, lfoValueHarmonic);
            }

            // Adjust effective depth based on input sensitivity:
            // If the input is loud, we reduce the effective modulation.
            float inputGain = std::abs(channelData[sample]);
            float adjustedDepth = depth * (1.0f - inputSensitivity * inputGain);

            // Compute modulation – for a delay-based vibrato engine you would modulate delay time.
            // Here, for simplicity, we modulate the amplitude (not a true pitch shift, but serves as an example).
            float modulatedSample = channelData[sample] * (1.0f + intensity * lfoValue * adjustedDepth);

            // Mix the dry and wet signals
            channelData[sample] = directLevel * channelData[sample] + effectLevel * modulatedSample;
        }
    }
}