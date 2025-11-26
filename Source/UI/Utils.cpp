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
		setSliderParams(slider, attachment, apvts, paramID, &label, parent, style);
	}

	void setSliderParams(juce::Slider& slider, std::unique_ptr<SliderAttachment>& attachment, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, juce::Label* label, juce::Component* parent, SliderStyle style) noexcept
	{
		slider.setSliderStyle(style);
		slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 5 * padding, static_cast<int>(2.5f * padding));

		attachment = std::make_unique<SliderAttachment>(apvts, paramID, slider);

		if (parent)
			parent->addAndMakeVisible(slider);

		if (label)
		{
			label->setColour(juce::Label::textColourId, juce::Colours::white);
			label->setFont(15.0f);
			label->setJustificationType(juce::Justification::centred);
			if (parent)
				parent->addAndMakeVisible(label);
		}
	}

	void setSliderBounds(juce::Slider& slider, int x, int y, int width, int height, juce::Label& label) noexcept
	{
		setSliderBounds(slider, x, y, width, height, &label);
	}

	void setSliderBounds(juce::Slider& slider, int x, int y, int width, int height, juce::Label* label) noexcept
	{
		if (label)
		{
			slider.setBounds(x, y, width, height - (padding * 2));
			label->setBounds(x, slider.getBottom(), width, padding * 2);
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
		if (parent)
			parent->addAndMakeVisible(comboBox);
	}

	// Attualmente superflua, ma coerente con le altre funzioni (setSliderBounds)
	void setComboBoxBounds(juce::ComboBox& comboBox, juce::Rectangle<int> size) noexcept
	{
		setComboBoxBounds(comboBox, size.getX(), size.getY(), size.getWidth(), size.getHeight());
	}

	void setComboBoxBounds(juce::ComboBox& comboBox, int x, int y, int width, int height) noexcept
	{
		comboBox.setBounds(x, y, width, height);
	}

	void setButton(juce::Button& button, const juce::String& buttonText, juce::Component* parent) noexcept
	{
		button.setButtonText(buttonText);
		button.setMouseCursor(juce::MouseCursor::PointingHandCursor);

		if (parent)
		{
			parent->addAndMakeVisible(button);
			if (auto* listener = dynamic_cast<juce::Button::Listener*>(parent))
				button.addListener(listener);
		}
	}

	// Attualmente superflua, ma coerente con le altre funzioni (setSliderBounds, setComboBoxBounds)
	void setButtonBounds(juce::Button& button, juce::Rectangle<int> size) noexcept
	{
		button.setBounds(size);
	}

	juce::Rectangle<int> getBoundsWithPadding(juce::Component* parent, int paddingOverride) noexcept
	{
		if (!parent)
			return {};

		const int usePadding = (paddingOverride == 0 ? utils::padding : paddingOverride);
		return parent->getLocalBounds().reduced(usePadding);
	}

	void drawBorders(juce::Graphics& g, juce::Component* parent, juce::Colour colour, juce::String title) noexcept
	{
		auto bounds = getBoundsWithPadding(parent);
		auto labelSpace = bounds.removeFromTop(2 * padding);

		g.setColour(colour);
		g.setFont(15.0f);
		g.drawText(title, labelSpace.withX(padding), juce::Justification::left);
		g.drawRoundedRectangle(bounds.toFloat(),
			static_cast<float>(padding) / 2.0f,
			static_cast<float>(padding) / 5.0f);
	}

	void layoutVisibleRow(int x, int y, int totalWidth, int height, std::initializer_list<LabeledSlider*> sliders) noexcept
	{
		// Conta quanti sono visibili
		int visibleCount = 0;
		for (auto* ls : sliders)
			if (ls && ls->slider.isVisible())
				++visibleCount;

		if (visibleCount == 0)
			return;

		// Larghezza per colonna, con padding tra colonne
		const int availableWidth = totalWidth - padding * (visibleCount - 1);
		const int columnWidth = availableWidth / visibleCount;

		int nextX = x;
		for (auto* ls : sliders)
		{
			if (ls && ls->slider.isVisible())
			{
				// utils::setSliderBounds(ls->slider, nextX, y, columnWidth, height, ls->label);
				ls->setBounds(nextX, y, columnWidth, height);
				nextX += columnWidth + padding;
			}
		}
	}
}