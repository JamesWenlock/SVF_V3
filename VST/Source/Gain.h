/*
  ==============================================================================

    Gain.h
    Created: 13 Jul 2020 10:15:19pm
    Author:  Admin

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

// Sets gain (with ramp)
class Gain
{
public:
    
    void init (float gain);
    void setGain (juce::AudioBuffer<float> &buffer, float gain);
    
private:
    float prevGain;
};
