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
	// DA RIMUOVERE : ora usato il DropDown in FXComponent
	void setComboBoxParams(juce::ComboBox& comboBox, std::unique_ptr<ComboBoxAttachment>& attachment, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, juce::StringArray choices, juce::Component* parent) noexcept
	{
		comboBox.addItemList(choices, 1);
		attachment = std::make_unique<ComboBoxAttachment>(apvts, paramID, comboBox);
		if (parent)
			parent->addAndMakeVisible(comboBox);
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

	juce::Rectangle<int> getBoundsWithPadding(juce::Component* parent, int paddingOverride) noexcept
	{
		if (!parent)
			return {};

		const int usePadding = (paddingOverride == 0 ? utils::padding : paddingOverride);
		return parent->getLocalBounds().reduced(usePadding);
	}

	// Area contenuti uniforme: padding esterno + rimozione dell'area del titolo + rientro interno
	juce::Rectangle<int> getContentArea(juce::Component* parent) noexcept
	{
		auto bounds = getBoundsWithPadding(parent);
		bounds.removeFromTop(titleAreaHeight());
		bounds.reduce(contentPadding, contentPadding);
		return bounds;
	}

	void drawBorders(juce::Graphics& g, juce::Component* parent, juce::Colour colour, juce::String title) noexcept
	{
		auto bounds = getBoundsWithPadding(parent);
		auto labelSpace = bounds.removeFromTop(titleAreaHeight());

		g.setColour(colour);
		g.setFont(15.0f);
		g.drawText(title, labelSpace.withX(padding), juce::Justification::left);
		g.drawRoundedRectangle(bounds.toFloat(),
			static_cast<float>(padding) / 2.0f,
			static_cast<float>(padding) / 5.0f);
	}

	void layoutVisibleRow(int x, int y, int totalWidth, int height, std::initializer_list<LabeledSlider*> sliders) noexcept
	{
		int visibleCount = 0;
		for (auto* ls : sliders)
			if (ls && ls->slider.isVisible())
				++visibleCount;

		if (visibleCount == 0)
			return;

		const int availableWidth = totalWidth - padding * (visibleCount - 1);
		const int columnWidth = availableWidth / visibleCount;

		int nextX = x;
		for (auto* ls : sliders)
		{
			if (ls && ls->slider.isVisible())
			{
				ls->setBounds(nextX, y, columnWidth, height, padding * 2, 0);
				nextX += columnWidth + padding;
			}
		}
	}

	void comboAndSliderRow(juce::ComboBox& comboBox, std::initializer_list<LabeledSlider*> sliders, juce::Component* parent, int x, int y, int totalWidth, int height) noexcept
	{
		auto content = getContentArea(parent);

		if (height == 0)
			height = content.getHeight() / 6;
		if (totalWidth == 0)
			totalWidth = content.getWidth();

		const int startX = content.getX();
		const int startY = content.getY();

		comboBox.setBounds(startX, startY, totalWidth, height);

		const int rowY = comboBox.getBottom() + padding;
		const int rowH = content.getBottom() - rowY; // tutto lo spazio rimanente

		layoutVisibleRow(startX, rowY, totalWidth, rowH, sliders);
	}
}