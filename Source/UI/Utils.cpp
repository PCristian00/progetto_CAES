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

	void setSliderParams(juce::Slider& slider, std::unique_ptr<SliderAttachment>& attachment, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, juce::Label& label, juce::Component* parent, SliderStyle style) noexcept
	{
		slider.setSliderStyle(style);
		slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);

		attachment = std::make_unique<SliderAttachment>(apvts, paramID, slider);

		if (parent != nullptr)
			parent->addAndMakeVisible(&slider);

		// Impostazioni label
		label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
		label.setFont(15.0f);
		label.setJustificationType(juce::Justification::centred);
		if (parent != nullptr)
			parent->addAndMakeVisible(&label);
	}

	// VALUTARE DI GESTIRE QUI LE DIMENSIONI STANDARD DEI BOUNDS (vedi nota OneNote 40)

	void setSliderBounds(juce::Slider& slider, juce::Label& label, int x, int y, int width, int height) noexcept
	{
		slider.setBounds(x, y, width, height);
		label.setBounds(x, y + 20, width, 20);
	}

	void setComboBoxParams(juce::ComboBox& comboBox, std::unique_ptr<ComboBoxAttachment>& attachment, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, juce::StringArray choices, juce::Component* parent) noexcept
	{
		comboBox.addItemList(choices, 1);

		attachment = std::make_unique<ComboBoxAttachment>(apvts, paramID, comboBox);

		if (parent != nullptr)
			parent->addAndMakeVisible(&comboBox);
	}
	void setComboBoxBounds(juce::ComboBox& comboBox, int x, int y, int width, int height) noexcept
	{
		comboBox.setBounds(x, y, width, height);
	}



	juce::Rectangle<int>& getBoundsWithPadding(juce::Component* parent) noexcept
	{
		return parent->getLocalBounds().reduced(padding);
	}

	void drawBorders(juce::Graphics& g, juce::Component* parent, juce::Colour colour, juce::String title) noexcept
	{
		juce::Rectangle<int> bounds = getBoundsWithPadding(parent);
		juce::Rectangle<int> labelSpace = bounds.removeFromTop(2 * padding);

		g.setColour(colour);
		g.setFont(15.0f);
		g.drawText(title, labelSpace.withX(padding), juce::Justification::left);
		g.drawRoundedRectangle(bounds.toFloat(), 5.0f, 2.0f);
	}
}