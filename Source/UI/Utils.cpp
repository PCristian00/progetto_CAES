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

	// MIGLIORARE (più parametri?) per modificare lo stile singolarmente (forse passare ad esempio Slider::LinearBarVertical etc.)

	// AGGIUNGERE LABEL, apvts, sliderID COME PARAMETRO

	void setSliderParams(juce::Slider& slider, std::unique_ptr<Attachment>& attachment, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, juce::Label& label, juce::Component* parent) noexcept
	{
		slider.setSliderStyle(juce::Slider::LinearBarVertical);
		slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);

		attachment = std::make_unique<Attachment>(apvts, paramID, slider);

		if (parent != nullptr)
			parent->addAndMakeVisible(&slider);

		// Impostazioni label
		label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
		label.setFont(15.0f);
		label.setJustificationType(juce::Justification::centred);
		if (parent != nullptr)
			parent->addAndMakeVisible(&label);
	}

	// AGGIUNGERE LABEL COME PARAMETRO?
	void setSliderBounds(juce::Slider& slider, juce::Label& label, int x, int y, int width, int height) noexcept
	{
		slider.setBounds(x, y, width, height);
		label.setBounds(slider.getX(), slider.getY() - 20, slider.getWidth(), 20);
	}
}