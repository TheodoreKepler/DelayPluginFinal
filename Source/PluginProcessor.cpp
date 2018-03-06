/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
DelayPluginAudioProcessor::DelayPluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    addParameter(wetdry = new AudioParameterFloat("wetdry","Wet/Dry",0.0f,1.0f,0.5f));
    addParameter(delaytime = new AudioParameterFloat("time","Delay Time",1.00f,2.00f,1.05f));
    addParameter(delayfdbk = new AudioParameterFloat("feedback","Delay Feedback",0.0f,1.0f,0.5f));
}

DelayPluginAudioProcessor::~DelayPluginAudioProcessor()
{
}

//==============================================================================
const String DelayPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DelayPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DelayPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DelayPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DelayPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DelayPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DelayPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DelayPluginAudioProcessor::setCurrentProgram (int index)
{
}

const String DelayPluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void DelayPluginAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DelayPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // hoping the number of inputs is already set ??
    numChannels = getTotalNumInputChannels();
    //if (numChannels != getTotalNumOutputChannels()) {
        //std::cerr << "Delay should have same number of ins and outs!" << std::endl;
    //}

    // need a buffer at least this long to make the delay
    blockSize = samplesPerBlock;
    sampleOffset = (int)floor(sampleRate * delaytime->get());
    delayBuffer.setSize(numChannels,sampleOffset);
    //feedbackBuffer.setSize(numChannels, sampleOffset);
    delayBuffer.clear();
    //feedbackBuffer.clear();

    windowIter = sampleOffset;
}

void DelayPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DelayPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

int DelayPluginAudioProcessor::nextIndexWrapAround(int i) {
  if (i >= delayBuffer.getNumSamples()-1) {
    return 0;
  } else {
    return i+1;
  }
}

int DelayPluginAudioProcessor::indexNSamplesBack(int curr, int i) {
  if (curr - i < 0) {
    return delayBuffer.getNumSamples() + (curr - i);
  } else {
    return i;
  }
}

void DelayPluginAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages) {
    // idk what this means, but i guess ill go with it
    ScopedNoDenormals noDenormals;

    //////// The commented out block is just a check that should always pass:
    // if(buffer.getNumSamples() != blockSize) {
    //   std::cerr << "The input buffer is not the gauranteed blocksize!!" << std::endl;
    // }

    float *monoDelayBuffer=0;
    float *inputBuffer=0;
    //float *inputBufferWrite=0;

    // copy input samples into delay buffer starting from
    //  offset until end, and wrap around after that to beginning
    for (int ch=0; ch<numChannels; ch++) {

        monoDelayBuffer = delayBuffer.getWritePointer(jmin(ch,delayBuffer.getNumChannels()-1));
        inputBuffer = buffer.getWritePointer(ch);

        for (int i=0; i<blockSize; i++) {
            float in = inputBuffer[i];
            monoDelayBuffer[windowIter] = in;
            float out = 0.0f;
            out = (in * (1-(*wetdry))) + (monoDelayBuffer[windowIter] * (*wetdry));
            monoDelayBuffer[windowLocation] = in + ((monoDelayBuffer[windowIter]) * (*delayfdbk));
            
            // incr
            windowIter = nextIndexWrapAround(windowIter);
            windowLocation = nextIndexWrapAround(windowLocation);
            
            // assign
            inputBuffer[i] = out;
        }
    }
}

//==============================================================================
bool DelayPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DelayPluginAudioProcessor::createEditor()
{
    return new DelayPluginAudioProcessorEditor (*this);
}

//==============================================================================
void DelayPluginAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DelayPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayPluginAudioProcessor();
}
