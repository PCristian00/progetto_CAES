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
	// Impostazioni Slider (GUI)
	void setSliderParams(juce::Slider& slider, std::unique_ptr<SliderAttachment>& attachment, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, juce::Label& label, juce::Component* parent = nullptr, SliderStyle style = SliderStyle::LinearBarVertical) noexcept;
	void setSliderBounds(juce::Slider& slider, juce::Label& label, int x, int y, int width, int height) noexcept;
	// Impostazioni ComboBox (GUI)
	void setComboBoxParams(juce::ComboBox& comboBox, std::unique_ptr<ComboBoxAttachment>& attachment, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, juce::StringArray choices, juce::Component* parent = nullptr) noexcept;
	void setComboBoxBounds(juce::ComboBox& comboBox, int x, int y, int width, int height) noexcept;

	juce::Rectangle<int>& getBoundsWithPadding(juce::Component* parent = nullptr) noexcept;

	const int padding = 10;
}
