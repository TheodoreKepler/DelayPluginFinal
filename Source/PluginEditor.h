/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class DelayPluginAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    DelayPluginAudioProcessorEditor (DelayPluginAudioProcessor&);
    ~DelayPluginAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    class ParameterSlider;
    Label wetdryLabel, delaytimeLabel, delayfdbkLabel;
    ScopedPointer<ParameterSlider> wetdrySlider, delaytimeSlider, delayfdbkSlider;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DelayPluginAudioProcessor& processor;
    
    //Slider wetdry;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayPluginAudioProcessorEditor)
};
