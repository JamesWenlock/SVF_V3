/*
  ==============================================================================

    OnePoll.h
    Created: 12 Jul 2020 12:13:44pm
    Author:  Admin

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

// One Poll filter for parameter smoothing
class OnePoll
{
public:
    // initializes OnePoll with given cutoff and sample
    void init(float cutoff, float sample);
    
    // process sample
    float processSample(float sample);
    
private:
    float a0, b1; // coeffs
    float y1; // previous output
};
