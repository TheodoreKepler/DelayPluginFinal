/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

class DelayPluginAudioProcessorEditor::ParameterSlider   : public Slider,
private Timer
{
public:
    ParameterSlider (AudioProcessorParameter& p)
    : Slider (p.getName (256)), param (p)
    {
        setRange (0.0, 1.0, 0.0);
        startTimerHz (30);
        updateSliderPos();
    }
    
    void valueChanged() override        { param.setValueNotifyingHost ((float) Slider::getValue()); }
    
    void timerCallback() override       { updateSliderPos(); }
    
    void startedDragging() override     { param.beginChangeGesture(); }
    void stoppedDragging() override     { param.endChangeGesture();   }
    
    double getValueFromText (const String& text) override   { return param.getValueForText (text); }
    String getTextFromValue (double value) override         { return param.getText ((float) value, 1024); }
    
    void updateSliderPos()
    {
        const float newValue = param.getValue();
        
        if (newValue != (float) Slider::getValue() && ! isMouseButtonDown())
            Slider::setValue (newValue, NotificationType::dontSendNotification);
    }
    
    AudioProcessorParameter& param;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterSlider)
};


//==============================================================================
DelayPluginAudioProcessorEditor::DelayPluginAudioProcessorEditor (DelayPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p),
      wetdryLabel(String(),"Wet/Dry"),
      delaytimeLabel(String(),"Delay Time"),
      delayfdbkLabel(String(),"Feedback")
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    //setSize (200, 200);
    
    // Wet dry knob placement
    //wetdry.setSliderStyle(juce::Slider::LinearVertical);
    //wetdry.setRange(1.00, 100.00, 1.00);
    //wetdry.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
    //wetdry.setPopupDisplayEnabled (true, false, this);
    //wetdry.setTextValueSuffix (" Wet/Dry");
    //wetdry.setValue(1.0);
    //addAndMakeVisible(&wetdry);
    
    // Slider shapes
    addAndMakeVisible(wetdrySlider = new ParameterSlider(*p.wetdry));
    wetdrySlider->setSliderStyle(Slider::LinearVertical);
    
    addAndMakeVisible(delaytimeSlider = new ParameterSlider(*p.delaytime));
    delaytimeSlider->setSliderStyle(Slider::LinearVertical);
    
    addAndMakeVisible(delayfdbkSlider = new ParameterSlider(*p.delayfdbk));
    delayfdbkSlider->setSliderStyle(Slider::LinearVertical);
    
    // Label attachment
    wetdryLabel.attachToComponent(wetdrySlider,true);
    wetdryLabel.setFont(Font(12.0f));
    
    delaytimeLabel.attachToComponent(delaytimeSlider,true);
    delaytimeLabel.setFont(Font(12.0f));
    
    delayfdbkLabel.attachToComponent(delayfdbkSlider,true);
    delayfdbkLabel.setFont(Font(12.0f));
    
    // set resize limits for this plug-in
    setResizeLimits (400, 200, 1024, 700);
    
    // set our component's initial size to be the last one that was stored in the filter's settings
    setSize (p.lastUIWidth,p.lastUIHeight);
}

DelayPluginAudioProcessorEditor::~DelayPluginAudioProcessorEditor()
{
}

//==============================================================================
void DelayPluginAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::ColourIds::backgroundColourId));

    g.setColour (Colours::darkcyan);
    // g.drawText("Delay Plugin!", getWidth()/2, getHeight()-5, 100, 20, juce::Justification::centredBottom);
    g.fillAll();
}

void DelayPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    //Rectangle<int> r (getLocalBounds().reduced (8));
    //r.removeFromTop (20);
    //Rectangle<int> sliderArea (r.removeFromTop (60));
    
    wetdrySlider->setBounds(getWidth()-40, 30, 20, getHeight()-60);
    delaytimeSlider->setBounds(40, 30, 20, getHeight()-60);
    delayfdbkSlider->setBounds(getWidth()/2, 30, 20, getHeight()-60);
    //delaySlider->setBounds (sliderArea.removeFromLeft (jmin (180, sliderArea.getWidth())));
    processor.lastUIWidth = getWidth();
    processor.lastUIHeight = getHeight();
}
