/*
  ==============================================================================

	Utils.h
	Created: 8 Nov 2025 5:39:14pm
	Author:  crist

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

// AGGIUNGERE ANCHE GESTIONE COMBOBOX (per ora solo in Osc)

using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
using SliderStyle = juce::Slider::SliderStyle;

namespace utils
{
	// Impostazioni Slider (GUI)
	void setSliderParams(juce::Slider& slider, std::unique_ptr<Attachment>& attachment, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, juce::Label& label, juce::Component* parent = nullptr, SliderStyle style = SliderStyle::LinearBarVertical) noexcept;
	void setSliderBounds(juce::Slider& slider, juce::Label& label, int x, int y, int width, int height) noexcept;
	juce::Rectangle<int>& getBoundsWithPadding(juce::Component* parent = nullptr) noexcept;

	const int padding = 10;
}
