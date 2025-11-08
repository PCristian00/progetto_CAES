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
	// Impostazioni Slider (GUI)
	void setSliderParams(juce::Slider& slider, juce::Component* parent = nullptr) noexcept;
	void setSliderBounds(juce::Slider& slider, int x, int y, int width, int height) noexcept;
}
