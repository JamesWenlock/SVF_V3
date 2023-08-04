/*
  ==============================================================================

    Gain.cpp
    Created: 13 Jul 2020 10:15:19pm
    Author:  Admin

  ==============================================================================
*/

#include "Gain.h"

void Gain::init (float gain)
{
    prevGain = gain;
}

void Gain::setGain (juce::AudioBuffer<float> &buffer, float gain)
{
    const float curAmp = juce::Decibels::decibelsToGain(gain);
    
    if (prevGain == gain)
    {
        buffer.applyGain (curAmp);
    }
    else
    {
        buffer.applyGainRamp (0, buffer.getNumSamples(), juce::Decibels::decibelsToGain(prevGain), curAmp);
        prevGain = gain;
    }
}
