/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SVFAudioProcessor::SVFAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParameters())

#endif
{
}

SVFAudioProcessor::~SVFAudioProcessor()
{
}

//==============================================================================
const juce::String SVFAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SVFAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SVFAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SVFAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SVFAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SVFAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SVFAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SVFAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SVFAudioProcessor::getProgramName (int index)
{
    return {};
}

void SVFAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SVFAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    const float cutoff = *apvts.getRawParameterValue("cutoff");
    const float q = *apvts.getRawParameterValue("q");
    const float gain = *apvts.getRawParameterValue("gain");
    const float onePollCutoff = 1;
    
    svf.init(cutoff, (float) sampleRate, q, onePollCutoff);
    g.init(gain);
}

void SVFAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SVFAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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

void SVFAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    std::atomic<float>* type = apvts.getRawParameterValue ("type");
    std::atomic<float>* gain = apvts.getRawParameterValue("gain");
    std::atomic<float>* cutoff = apvts.getRawParameterValue("cutoff");
    std::atomic<float>* q  = apvts.getRawParameterValue ("q");
    svf.updateVars(*cutoff, (float) getSampleRate(),  *q);

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    g.setGain(buffer, *gain);

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        for (int sample = 0; sample < buffer.getNumSamples(); sample++)
        {
            svf.processSample(channelData[sample], channel);
            channelData[sample] = svf.getSample((int) *type, channel);
        }
    }
}

//==============================================================================
bool SVFAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SVFAudioProcessor::createEditor()
{
    return new SVFAudioProcessorEditor (*this);
}

//==============================================================================
void SVFAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = apvts.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void SVFAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (apvts.state.getType()))
            apvts.replaceState (ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SVFAudioProcessor();
}

AudioProcessorValueTreeState::ParameterLayout SVFAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> parameters;
    
    parameters.push_back (std::make_unique<AudioParameterFloat> (juce::ParameterID("cutoff", 1),                                // parameter ID
                                                                 "Cutoff",                                // parameter name
                                                                 NormalisableRange<float> (20.0f, 6000.0f, 0.01f, 0.5f, false), // parameter range
                                                                 6000.0f));
    parameters.push_back (std::make_unique<AudioParameterFloat> (juce::ParameterID("q", 2),                                // parameter ID
                                                                 "Q",                                // parameter name
                                                                 NormalisableRange<float> (0.8f, 15.0f, 0.01f, 1.0f, false), // parameter range
                                                                 0.5f));
    parameters.push_back (std::make_unique<AudioParameterInt> (juce::ParameterID("type", 3), "Type", 0, 3, 0, "Filter Type", nullptr, nullptr));
    parameters.push_back (std::make_unique<AudioParameterFloat> (juce::ParameterID("gain", 4),                                // parameter ID
                                                                 "gain",                                // parameter name
                                                                 NormalisableRange<float> (-30.0f, 0.0f), // parameter range
                                                                 0.0f
                                                                 ));
    return { parameters.begin(), parameters.end() };
}
