/*
  ==============================================================================

    OnePoll.cpp
    Created: 12 Jul 2020 12:13:44pm
    Author:  Admin

  ==============================================================================
*/

#include "OnePoll.h"

// initializes OnePoll with given cutoff and sample
void OnePoll::init(float cutoff, float val)
{
    b1 = exp(-2.0f * M_PI * cutoff);
    a0 = 1.0f - b1;

    processSample(val);
}

float OnePoll::processSample(float sample)
{
    const float y = (a0 * sample) + (b1 * y1);
    y1 = y;
    return y;
}
