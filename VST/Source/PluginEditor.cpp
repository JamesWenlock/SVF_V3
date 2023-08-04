/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SVFAudioProcessorEditor::SVFAudioProcessorEditor (SVFAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (500, 300);
    reduction = 5;
    
    String cutoffParamID;
    int cutoffIndex;
    cutoffParamID = "cutoff";
    cutoffIndex = 1;
    initSlider(cutoffParamID, cutoffIndex, &cutoffAttachment);
    
    String qParamID;
    int qIndex;
    qParamID = "q";
    qIndex = 2;
    initSlider(qParamID, qIndex, &qAttachment);
    
    // define state knob
    String typeParamID;
    int typeIndex;
    typeParamID = "type";
    typeIndex = 3;
    initSlider(typeParamID, typeIndex, &typeAttachment);
    
    String gainParamID;
    int gainIndex;
    gainParamID = "gain";
    gainIndex = 0;
    initSlider(gainParamID, gainIndex, &gainAttachment);

    calculateBounds();
}

SVFAudioProcessorEditor::~SVFAudioProcessorEditor()
{
}

//==============================================================================
void SVFAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::black);
    g.setColour (Colours::yellow);
    g.setFont (Font (Font::getDefaultMonospacedFontName(), titleFontSize, Font::plain));
    
    for (int i = 0; i < knobArraySize; i++)
    {
        knobArray[i].drawOutline(g);
    }
    
    g.setColour(Colours::white);
    g.setFont (Font (Font::getDefaultMonospacedFontName(), titleFontSize, Font::bold));
    g.drawFittedText ("Chamberlin SVF V_2", title, Justification::bottomLeft, 1);
    
}

void SVFAudioProcessorEditor::sliderValueChanged(Slider *slider)
{
    if (slider == &knobArray[knobMap["cutoff"]])
    {
        setSliderText("cutoff", 1.0f);
    }
    if (slider == &knobArray[knobMap["q"]])
    {
        setSliderText("q", 0.1f);
    }
    if (slider == &knobArray[knobMap["type"]])
    {
        setSliderText("type", -1.0f);
    }
    if (slider == &knobArray[knobMap["gain"]])
    {
        setSliderText("gain", 1.0f);
    }
}

void SVFAudioProcessorEditor::resized()
{
    this->calculateBounds();
}

void SVFAudioProcessorEditor::setSliderText(String paramID, float resolution)
{
    if (paramID.equalsIgnoreCase("type"))
    {
        changeTypeText(knobArray[knobMap[paramID]].getValue());
    }
    else
    {
        float value = knobArray[knobMap[paramID]].getValue();
        value = round(value * (1.0f / resolution)) * resolution;
        knobArray[knobMap[paramID]].value.setText((String) value , dontSendNotification);
    }
}

void SVFAudioProcessorEditor::calculateBounds()
{
    // rectangle containing local bounds
    Rectangle<int> r = getLocalBounds();
    
    titleHeight = r.getHeight() / 4;
    
    title = r.removeFromTop(titleHeight);
    title.reduce(reduction * 6, reduction * 2);
    titleFontSize = title.getHeight() * 0.65;
    r.removeFromBottom(titleHeight / 4);
    r.reduce(reduction * 2, reduction);
    int knobWidth = r.getWidth() / knobArraySize;
    
    for (int i = 0; i < knobArraySize; i++)
    {
        Rectangle<int> thisRect;
        if (r.getWidth() > knobWidth)
        {
            thisRect = r.removeFromLeft(knobWidth);
        }
        else
        {
            thisRect = r;
        }
        thisRect.reduce(reduction, reduction);
        knobArray[i].setComponentBounds(thisRect);
    }
}


void SVFAudioProcessorEditor::initSlider(String paramID, int index, std::unique_ptr<SliderAttachment> * attachment)
{
    addAndMakeVisible(knobArray[index]);
    addAndMakeVisible(knobArray[index].value);
    knobMap.set(paramID, index);
    *attachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.apvts, paramID, knobArray[index]);
    knobArray[index].init(paramID);
    knobArray[index].addListener(this);
    setSliderText(paramID, 1.0f);
}

void SVFAudioProcessorEditor::changeTypeText(int typeValue)
{
    switch(typeValue)
    {
        case 0:
            changeTypeText("low");
            break;
        case 1:
            changeTypeText("band");
            break;
        case 2:
            changeTypeText("notch");
            break;
        case 3:
            changeTypeText("high");
            break;
        default:
            changeTypeText("low");
    }
}

void SVFAudioProcessorEditor::changeTypeText(juce::String typeName)
{
    knobArray[knobMap["type"]].value.setText(typeName, dontSendNotification);
}

