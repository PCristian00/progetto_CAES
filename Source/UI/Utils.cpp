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

	// Overload retrocompatibile: delega alla versione con puntatore
	void setSliderParams(juce::Slider& slider, std::unique_ptr<SliderAttachment>& attachment, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, juce::Label& label, juce::Component* parent, SliderStyle style) noexcept
	{
		setSliderParams(slider, attachment, apvts, paramID, &label, parent, style);
	}

	// Nuova implementazione con label opzionale
	void setSliderParams(juce::Slider& slider, std::unique_ptr<SliderAttachment>& attachment, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, juce::Label* label, juce::Component* parent, SliderStyle style) noexcept
	{
		slider.setSliderStyle(style);
		slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 5 * padding, 2.5f * padding);

		attachment = std::make_unique<SliderAttachment>(apvts, paramID, slider);

		if (parent != nullptr)
			parent->addAndMakeVisible(&slider);

		// Impostazioni label (se presente)
		if (label != nullptr)
		{
			label->setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
			label->setFont(15.0f);
			label->setJustificationType(juce::Justification::centred);
			if (parent != nullptr)
				parent->addAndMakeVisible(label);
		}
	}

	// Overload retrocompatibile: delega alla versione con puntatore
	void setSliderBounds(juce::Slider& slider, int x, int y, int width, int height, juce::Label& label) noexcept
	{
		setSliderBounds(slider, x, y, width, height, &label);
	}

	// Nuova implementazione con label opzionale
	void setSliderBounds(juce::Slider& slider, int x, int y, int width, int height, juce::Label* label) noexcept
	{
		if (label != nullptr)
		{
			slider.setBounds(x, y, width, height - (padding * 2));
			label->setBounds(x, slider.getBottom(), width, (padding * 2));
		}
		else
		{
			slider.setBounds(x, y, width, height);
		}
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



	juce::Rectangle<int>& getBoundsWithPadding(juce::Component* parent, int padding) noexcept
	{
		if (padding == 0)
			padding = utils::padding;

		return parent->getLocalBounds().reduced(padding);
	}

	void drawBorders(juce::Graphics& g, juce::Component* parent, juce::Colour colour, juce::String title) noexcept
	{
		juce::Rectangle<int> bounds = getBoundsWithPadding(parent);
		juce::Rectangle<int> labelSpace = bounds.removeFromTop(2 * padding);

		g.setColour(colour);
		g.setFont(15.0f);
		g.drawText(title, labelSpace.withX(padding), juce::Justification::left);
		g.drawRoundedRectangle(bounds.toFloat(), static_cast<float>(padding) / 2, static_cast<float>(padding) / 5);
	}
}