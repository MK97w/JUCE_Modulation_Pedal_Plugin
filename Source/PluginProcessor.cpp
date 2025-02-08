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
    vibrato.fetchParametersFromAPVTS(params);
    // Process the buffer using the vibrato engine
    vibrato.processBlock(buffer, midiMessages ,getSampleRate());
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
    auto vibratoGroup = vibrato.createVibratoParameterGroup();

    auto flangerGroup = std::make_unique<juce::AudioProcessorParameterGroup>("flanger", "Flanger", "|");
    flangerGroup->addChild(std::make_unique<juce::AudioParameterFloat>("_Flanger_A", "Flanger A", 0.1f, 10.0f, 5.0f));
    flangerGroup->addChild(std::make_unique<juce::AudioParameterBool>("_Flanger_B", "Flanger B", false));
    vibratoGroup->addChild(std::make_unique<juce::AudioParameterFloat>("_Flanger_C", "Flanger C", 0.0f, 1.0f, 0.5f));

    auto modulationGroup = std::make_unique<juce::AudioProcessorParameterGroup>("modulation", "Modulation", "|");
    modulationGroup->addChild(std::move(vibratoGroup));
    modulationGroup->addChild(std::move(flangerGroup));



    return { std::move(modulationGroup) };
}