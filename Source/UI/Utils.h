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

	const int padding = 10;  // default = 10
	const int Xstart = padding * 1.5f;
	const int Ystart = padding * 4;

	// Impostazioni Slider (GUI)
	void setSliderParams(juce::Slider& slider, std::unique_ptr<SliderAttachment>& attachment, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, juce::Label& label, juce::Component* parent = nullptr, SliderStyle style = SliderStyle::LinearBarVertical) noexcept;
	// overload: label opzionale (default = nullptr)
	void setSliderParams(juce::Slider& slider, std::unique_ptr<SliderAttachment>& attachment, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, juce::Label* label = nullptr, juce::Component* parent = nullptr, SliderStyle style = SliderStyle::LinearBarVertical) noexcept;

	// Impostazioni bounds Slider (GUI)
	void setSliderBounds(juce::Slider& slider, int x, int y, int width, int height, juce::Label& label) noexcept;
	// overload: label opzionale (default = nullptr)
	void setSliderBounds(juce::Slider& slider, int x, int y, int width, int height, juce::Label* label = nullptr) noexcept;

	// Impostazioni ComboBox (GUI)
	void setComboBoxParams(juce::ComboBox& comboBox, std::unique_ptr<ComboBoxAttachment>& attachment, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, juce::StringArray choices, juce::Component* parent = nullptr) noexcept;
	void setComboBoxBounds(juce::ComboBox& comboBox, int x, int y, int width, int height) noexcept;



	juce::Rectangle<int>& getBoundsWithPadding(juce::Component* parent = nullptr, int padding = 0) noexcept;
	void drawBorders(juce::Graphics& g, juce::Component* parent, juce::Colour colour, juce::String title = "") noexcept;
}
