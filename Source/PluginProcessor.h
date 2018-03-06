/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
/**
*/
class DelayPluginAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    DelayPluginAudioProcessor();
    ~DelayPluginAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>& buffer, MidiBuffer& midibuffer) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //=== Custom Stuff ===
    AudioParameterFloat *wetdry = nullptr;
    AudioParameterFloat *delaytime = nullptr;
    AudioParameterFloat *delayfdbk = nullptr;
    
    // these are used to persist the UI's size - the values are stored along with the
    // filter's other parameters, and the UI component will update them when it gets
    // resized.
    int lastUIWidth = 400, lastUIHeight = 200;

private:
    //==============================================================================
    AudioBuffer<float> delayBuffer;
    //AudioBuffer<float> feedbackBuffer;
    int numChannels = 1;
    int sampleOffset = 0;
    int blockSize = 0;
    int windowIter = 0;
    int windowLocation = 0;
    int nextIndexWrapAround(int i);
    int indexNSamplesBack(int curr, int i);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayPluginAudioProcessor)
};
