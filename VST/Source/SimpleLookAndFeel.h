/*
  ==============================================================================

    SimpleKnob.h
    Created: 25 Jun 2020 9:39:08pm
    Author:  Admin

  ==============================================================================
*/

#include <JuceHeader.h>

#pragma once

using namespace juce;

class SimpleLookAndFeel : public LookAndFeel_V4
{
public:
    void drawRotarySlider (Graphics &g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, Slider &slider) override;
private:
    Colour knobColor {Colours::yellow};
};
