/*
  ==============================================================================

    SVF.h
    Created: 12 Jul 2020 12:12:57pm
    Author:  Admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "OnePoll.h"

// State Variable Filter
class SVF 
{
public:
    
    // initializes SVF with given cutoff, samplerate, and q
    void init(float cutoff, float sampleRate, float q, float onePollCutoff);
    
    // updates f1 and q1 using given cutoff, sampleRate, q
    void updateVars(float cutoff, float sampleRate, float q);
    
    // updates f1 and q1 using given cutoff, sampleRate, q
    void updateVars(float cutoff, float sampleRate, float q, float onePollCutoff);
    
    // process sample for low, high, band, and notch for given channel
    void processSample(float sample, int channel);
    
    // get filter output for given channel
    // type 0 -> lowpass
    // type 1 -> bandpass
    // type 2 -> notch
    // type 3 -> highpass
    float getSample(int type, int channel);
    
private:
    // internal variables generated from parameters
    OnePoll f1Filter, q1Filter;
    float f1, q1;
    // state variables
    float d1 [2];
    float d2 [2];
    
    // output samples corresponding to lowpass, bandpass, highpass, and notch outputs
    float low [2], band [2], high [2], notch [2];
};
