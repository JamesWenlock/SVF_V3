/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "SimpleKnob.h"

//==============================================================================
/**
*/
class SVFAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                    public Slider::Listener

{
public:
    SVFAudioProcessorEditor (SVFAudioProcessor&);
    ~SVFAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    void sliderValueChanged (Slider* slider) override;
    void initSlider(String paramID, int index, std::unique_ptr<SliderAttachment> * attachment);
    void setSliderText(String paramID, float resolution);
    void calculateBounds();
    void changeTypeText(int typeValue);
    void changeTypeText(juce::String typeName);
    
    SVFAudioProcessor& processor;
    SimpleLookAndFeel simpleLookAndFeel;
    
    int knobArraySize {4};
    SimpleKnob knobArray [4];
        
    std::unique_ptr<SliderAttachment> cutoffAttachment;
    std::unique_ptr<SliderAttachment> qAttachment;
    std::unique_ptr<SliderAttachment> typeAttachment;
    std::unique_ptr<SliderAttachment> gainAttachment;

    HashMap<String, int> knobMap;
    Rectangle<int> title, infoLeft, infoRight;
    Label mStateText;
    String stateString;
    int titleHeight, reduction;
    float titleFontSize;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SVFAudioProcessorEditor)
};
