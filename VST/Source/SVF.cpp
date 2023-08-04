/*
  ==============================================================================

    SVF.cpp
    Created: 12 Jul 2020 12:12:57pm
    Author:  Admin

  ==============================================================================
*/

#include "SVF.h"

// initializes SVF with given cutoff, samplerate, and q
void SVF::init(float cutoff, float sampleRate, float q, float onePollCutoff)
{
    updateVars(cutoff, sampleRate, q, onePollCutoff);
    
    // clear fields
    for (int i = 0; i < 2; i++)
    {
        d1[i] = 0.0f;
        d2[i] = 0.0f;
        low[i] = 0.0f;
        high[i] = 0.0f;
        band[i] = 0.0f;
        notch[i] = 0.0f;
    };
}

// updates f1 and q1 using given cutoff, sampleRate, q
void SVF::updateVars(float cutoff, float sampleRate, float q)
{
    f1 = 2.f * sinf(M_PI * cutoff / sampleRate);
    q1 = 1.f / q;
}

// updates f1 and q1 using given cutoff, sampleRate, q
void SVF::updateVars(float cutoff, float sampleRate, float q, float onePollCutoff)
{
    updateVars(cutoff, sampleRate, q);
    this->f1Filter.init(onePollCutoff, f1);
    this->q1Filter.init(onePollCutoff, q1);
}

// process sample for low, high, band, and notch for given channel
void SVF::processSample(float sample, int channel)
{
    f1 = f1Filter.processSample(f1);
    q1 = q1Filter.processSample(q1);
    low[channel] = d2[channel] + (f1 * d1[channel]);
    high[channel] = sample - low[channel] - (q1 * d1[channel]);
    band[channel] = (f1 * high[channel]) + d1[channel];
    notch[channel] = high[channel] + low[channel];
    
    // store outputs into states
    d1[channel] = band[channel];
    d2[channel] = low[channel];
}

// get filter output for given channel
// type 0 -> lowpass
// type 1 -> bandpass
// type 2 -> notch
// type 3 -> highpass
float SVF::getSample(int type, int channel)
{
    switch(type)
    {
        case 0:
            return low[channel];
            break;
        case 1:
            return band[channel];
            break;
        case 2:
            return notch[channel];
            break;
        case 3:
            return high[channel];
            break;
        default:
            return low[channel];
    }
}
