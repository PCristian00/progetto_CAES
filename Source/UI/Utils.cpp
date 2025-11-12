/*
 ==============================================================================

   Utils.cpp
   Created: 8 Nov 2025 5:39:14pm
   Author:  crist

 ==============================================================================
*/

#include "Utils.h"

namespace utils
{

	// AGGIUNGERE LABEL COME PARAMETRO

	void setSliderParams(juce::Slider& slider, juce::Component* parent) noexcept
	{
		slider.setSliderStyle(juce::Slider::LinearBarVertical);
		slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);

		if (parent != nullptr)
			parent->addAndMakeVisible(&slider);
	}

	// AGGIUNGERE LABEL COME PARAMETRO?
	void setSliderBounds(juce::Slider& slider, int x, int y, int width, int height) noexcept
	{
		slider.setBounds(x, y, width, height);
	}
}