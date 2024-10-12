/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Modulation_Pedal_PluginAudioProcessor::Modulation_Pedal_PluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), params(*this, nullptr, "Parameters", createParameters())
#endif
{
}

Modulation_Pedal_PluginAudioProcessor::~Modulation_Pedal_PluginAudioProcessor()
{
}

//==============================================================================
const juce::String Modulation_Pedal_PluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Modulation_Pedal_PluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Modulation_Pedal_PluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Modulation_Pedal_PluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Modulation_Pedal_PluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Modulation_Pedal_PluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Modulation_Pedal_PluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Modulation_Pedal_PluginAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Modulation_Pedal_PluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void Modulation_Pedal_PluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Modulation_Pedal_PluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    auto delayBufferSize = 2.0 * sampleRate;
    delayBuffer.setSize(getTotalNumOutputChannels(), static_cast<int>(delayBufferSize));  
}

void Modulation_Pedal_PluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Modulation_Pedal_PluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Modulation_Pedal_PluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        fillBuffer(buffer, channel);
        readFromBuffer(buffer, delayBuffer, channel);
        feedbackBuffer(buffer, channel);
    }

    updateBufferPositions(buffer, delayBuffer);
}
void Modulation_Pedal_PluginAudioProcessor::fillBuffer(juce::AudioBuffer<float>& buffer, int channel)
{
    auto bufferSize = buffer.getNumSamples();
    auto delayBufferSize = delayBuffer.getNumSamples();
    auto* wet = params.getRawParameterValue("DRYWET");

    buffer.applyGain(0, bufferSize, 1.0f - (wet->load() / 100.0f));

    // Check to see if main buffer copies to delay buffer without needing to wrap...
    if (delayBufferSize >= bufferSize + writePosition)
    {
        // copy main buffer contents to delay buffer
        delayBuffer.copyFrom(channel, writePosition, buffer.getWritePointer(channel), bufferSize);
    }
    // if no
    else
    {
        // Determine how much space is left at the end of the delay buffer
        auto numSamplesToEnd = delayBufferSize - writePosition;

        // Copy that amount of contents to the end...
        delayBuffer.copyFrom(channel, writePosition, buffer.getWritePointer(channel), numSamplesToEnd);

        // Calculate how much contents is remaining to copy
        auto numSamplesAtStart = bufferSize - numSamplesToEnd;

        // Copy remaining amount to beginning of delay buffer
        delayBuffer.copyFrom(channel, 0, buffer.getWritePointer(channel, numSamplesToEnd), numSamplesAtStart);
    }
}

void Modulation_Pedal_PluginAudioProcessor::feedbackBuffer(juce::AudioBuffer<float>& buffer, int channel)
{
    auto bufferSize = buffer.getNumSamples();
    auto delayBufferSize = delayBuffer.getNumSamples();
    // feedback
    auto fbLeft = params.getRawParameterValue("FEEDBACKLEFT")->load();
    auto fbRight = params.getRawParameterValue("FEEDBACKRIGHT")->load();

    if (params.getRawParameterValue("FBLINK")->load() == true)
    {
        fbRight = fbLeft;
    }

    auto fb = channel == 0 ? fbLeft : fbRight;

    // Check to see if main buffer copies to delay buffer without needing to wrap...
    if (delayBufferSize >= bufferSize + writePosition)
    {
        // copy main buffer contents to delay buffer
        delayBuffer.addFromWithRamp(channel, writePosition, buffer.getWritePointer(channel), bufferSize, fb, fb);
    }
    // if no
    else
    {
        // Determine how much space is left at the end of the delay buffer
        auto numSamplesToEnd = delayBufferSize - writePosition;

        // Copy that amount of contents to the end...
        delayBuffer.addFromWithRamp(channel, writePosition, buffer.getWritePointer(channel), numSamplesToEnd, fb, fb);

        // Calculate how much contents is remaining to copy
        auto numSamplesAtStart = bufferSize - numSamplesToEnd;

        // Copy remaining amount to beginning of delay buffer
        delayBuffer.addFromWithRamp(channel, 0, buffer.getWritePointer(channel, numSamplesToEnd), numSamplesAtStart, fb, fb);
    }
}

void Modulation_Pedal_PluginAudioProcessor::readFromBuffer(juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& delayBuffer, int channel)
{
    auto bufferSize = buffer.getNumSamples();
    auto delayBufferSize = delayBuffer.getNumSamples();

    auto percent = params.getRawParameterValue("DRYWET")->load();
    auto g = juce::jmap(percent, 0.0f, 100.0f, 0.0f, 1.0f);
    auto dryGain = 1.0f - g;

    auto delayTimeLeft = params.getRawParameterValue("DELAYMSLEFT")->load();
    auto delayTimeRight = params.getRawParameterValue("DELAYMSRIGHT")->load();

    if (params.getRawParameterValue("DELAYLINK")->load() == true)
    {
        delayTimeRight = delayTimeLeft;
    }

    auto delayTime = channel == 0 ? delayTimeLeft : delayTimeRight;

    // delayMs
    auto readPosition = std::round(writePosition - (getSampleRate() * delayTime / 1000.0f));

    if (readPosition < 0)
        readPosition += delayBufferSize;

    buffer.applyGainRamp(0, bufferSize, dryGain, dryGain);

    if (readPosition + bufferSize < delayBufferSize)
    {
        buffer.addFromWithRamp(channel, 0, delayBuffer.getReadPointer(channel, readPosition), bufferSize, g, g);
    }
    else
    {
        auto numSamplesToEnd = delayBufferSize - readPosition;
        buffer.addFromWithRamp(channel, 0, delayBuffer.getReadPointer(channel, readPosition), numSamplesToEnd, g, g);

        auto numSamplesAtStart = bufferSize - numSamplesToEnd;
        buffer.addFromWithRamp(channel, numSamplesToEnd, delayBuffer.getReadPointer(channel, 0), numSamplesAtStart, g, g);
    }
}

void Modulation_Pedal_PluginAudioProcessor::updateBufferPositions(juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& delayBuffer)
{
    auto bufferSize = buffer.getNumSamples();
    auto delayBufferSize = delayBuffer.getNumSamples();

    writePosition += bufferSize;
    writePosition %= delayBufferSize;
}
//==============================================================================
bool Modulation_Pedal_PluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Modulation_Pedal_PluginAudioProcessor::createEditor()
{
    return new Modulation_Pedal_PluginAudioProcessorEditor (*this);
    //return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void Modulation_Pedal_PluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Modulation_Pedal_PluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Modulation_Pedal_PluginAudioProcessor();
}



juce::AudioProcessorValueTreeState::ParameterLayout 
Modulation_Pedal_PluginAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>("DELAYMSLEFT", "DELAY MS LEFT", 0.0f, 2000.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DELAYMSRIGHT", "DELAY MS RIGHT", 0.0f, 2000.0f, 0.0f));

    params.push_back(std::make_unique<juce::AudioParameterBool>("DELAYLINK", "DELAY LINK", false));


    params.push_back(std::make_unique<juce::AudioParameterFloat>("FEEDBACKLEFT", "FEEDBACK LEFT", 0.0f, 1.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FEEDBACKRIGHT", "FEEDBACK RIGHT", 0.0f, 1.0f, 0.0f));

    params.push_back(std::make_unique<juce::AudioParameterBool>("FBLINK", "FEEDBACK LINK", false));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("DRYWET", "DRY/WET", 0.0f, 100.0f, 0.0f));

    return { params.begin(), params.end() };
}