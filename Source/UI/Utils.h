/*
  ==============================================================================

	Utils.h
	Created: 8 Nov 2025 5:39:14pm
	Author:  crist

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace utils
{
	void setSliderParams(juce::Slider& slider);
	void setSliderBounds(juce::Slider& slider, int x, int y, int width, int height);
}
