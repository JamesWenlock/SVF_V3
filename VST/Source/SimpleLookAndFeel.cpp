/*
  ==============================================================================

    SimpleKnob.cpp
    Created: 25 Jun 2020 9:39:08pm
    Author:  Admin

  ==============================================================================
*/
#include <JuceHeader.h>
#include "SimpleLookAndFeel.h"

void SimpleLookAndFeel::drawRotarySlider (Graphics &g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, Slider &slider)
{
    auto diameter = jmin(width, height);
    auto reduction = diameter / 7;
    diameter = diameter - reduction;
    auto radius = diameter / 2;
    auto centerX = x + width / 2;
    auto centerY = y + height / 2;
    auto angle = rotaryStartAngle + (rotaryEndAngle - rotaryStartAngle) * sliderPos;
    auto dialDiameter = diameter / 10;
    auto dialRadius = dialDiameter / 2;
    auto dialTickPos = - 0.75;
    auto lineThickness = 2;
    g.setColour(knobColor);
    g.drawEllipse(centerX - radius, centerY - radius, diameter, diameter, lineThickness);
    Path dialTick;
    dialTick.addEllipse(0 - dialRadius, radius * dialTickPos - dialRadius, dialDiameter, dialDiameter);
    g.fillPath(dialTick, AffineTransform::rotation(angle).translated(centerX, centerY));
}
