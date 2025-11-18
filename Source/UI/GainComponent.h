/*
  ==============================================================================

	GainComponent.h
	Created: 8 Nov 2025 6:56:15pm
	Author:  crist

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class GainComponent : public juce::Component
{
public:
	GainComponent(juce::AudioProcessorValueTreeState& apvts, juce::String gainID);
	~GainComponent() override;

	void paint(juce::Graphics&) override;
	void resized() override;

private:
	juce::Slider gainSlider;

	using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
	std::unique_ptr<SliderAttachment> gainSliderAttachment;

	// juce::Label gainLabel{ "Gain", "Gain" }; Non usata per GainSlider: usa direttamente il titolo di utils::drawBorders

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainComponent)
};
