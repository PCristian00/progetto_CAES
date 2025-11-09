/*
  ==============================================================================

    OscData.cpp
    Created: 9 Nov 2025 4:43:23pm
    Author:  crist

  ==============================================================================
*/

#include "OscData.h"

void OscData::setWaveType(const int choice)
{
    switch (choice)
    {
    case 0:
		initialise([](float x) { return std::sin(x); }); // Sine wave
        break;
    case 1:
		initialise([](float x) { return x / juce::MathConstants<float>::pi; }); // Saw wave
        break;
    case 2:
		initialise([](float x) { return x < 0.0f ? -1.0f : 1.0f; }); // Square wave
        break;
    default:
        jassertfalse; // Unknown wave type!
        break;
    }
}