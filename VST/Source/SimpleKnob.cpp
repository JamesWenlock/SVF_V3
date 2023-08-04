/*
  ==============================================================================

    SimpleKnob.cpp
    Created: 26 Jun 2020 1:32:51pm
    Author:  Admin

  ==============================================================================
*/

#include "SimpleKnob.h"

SimpleKnob::~SimpleKnob()
{
    this->setLookAndFeel(nullptr);
}

void SimpleKnob::init(String name)
{
    this->setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    this->setTextBoxStyle(TextEntryBoxPosition::NoTextBox, true, 0, 0);
    this->setLookAndFeel(&lookAndFeel);
    this->name = name;
}

void SimpleKnob::setComponentBounds(Rectangle<int> bounds)
{
    paramTextHeight = bounds.getHeight() / 7;
    boundMap.set("name", bounds.removeFromTop(paramTextHeight));
    boundMap.set("value", bounds.removeFromBottom(paramTextHeight));
    boundMap.set("knob", bounds);
    this->setBounds(boundMap["knob"]);
    this->value.setBounds(boundMap["value"]);
    this->nameFontSize = boundMap["name"].getHeight() * 0.6;
    this->value.setFont(this->nameFontSize * 0.8);
    this->value.setJustificationType(Justification::centred);
}

void SimpleKnob::drawOutline (Graphics& g)
{
    g.setColour (Colours::yellow);
    for (HashMap<String, Rectangle<int>>::Iterator i (boundMap); i.next();)
        g.drawRect(i.getValue());
    g.setColour (Colours::white);
    g.setFont ((float) nameFontSize);
    g.drawFittedText(this->name, boundMap["name"], Justification::centred, 1);
}


