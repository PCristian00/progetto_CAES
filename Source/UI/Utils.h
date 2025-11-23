/*
  ==============================================================================

	Utils.h
	Created: 8 Nov 2025 5:39:14pm
	Author:  crist

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
using SliderStyle = juce::Slider::SliderStyle;

namespace utils
{
	const int padding = 10;
	const int Xstart = static_cast<int>(padding * 1.5f);
	const int Ystart = padding * 4;

	void setSliderParams(juce::Slider& slider, std::unique_ptr<SliderAttachment>& attachment, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, juce::Label& label, juce::Component* parent = nullptr, SliderStyle style = SliderStyle::LinearBarVertical) noexcept;

	void setSliderParams(juce::Slider& slider, std::unique_ptr<SliderAttachment>& attachment, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, juce::Label* label = nullptr, juce::Component* parent = nullptr, SliderStyle style = SliderStyle::LinearBarVertical) noexcept;

	void setSliderBounds(juce::Slider& slider, int x, int y, int width, int height, juce::Label& label) noexcept;
	void setSliderBounds(juce::Slider& slider, int x, int y, int width, int height, juce::Label* label = nullptr) noexcept;

	void setComboBoxParams(juce::ComboBox& comboBox, std::unique_ptr<ComboBoxAttachment>& attachment, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, juce::StringArray choices, juce::Component* parent = nullptr) noexcept;

	void setComboBoxBounds(juce::ComboBox& comboBox, juce::Rectangle<int> size) noexcept;
	void setComboBoxBounds(juce::ComboBox& comboBox, int x, int y, int width, int height) noexcept;

	void setButton(juce::Button& button, const juce::String& buttonText, juce::Component* parent = nullptr) noexcept;
	void setButtonBounds(juce::Button& button, juce::Rectangle<int> size) noexcept;

	juce::Rectangle<int> getBoundsWithPadding(juce::Component* parent = nullptr, int paddingOverride = 0) noexcept;

	void drawBorders(juce::Graphics& g, juce::Component* parent, juce::Colour colour, juce::String title = "") noexcept;
}
