/*
  ==============================================================================

	FXComponent.h
	Created: 25 Nov 2025 12:49:57pm
	Author:  crist

  ==============================================================================
*/
#pragma once
#include <JuceHeader.h>
#include "Utils.h"

class FXComponent : public juce::Component, private juce::ComboBox::Listener
{
	using LabeledSlider = utils::LabeledSlider;
	using APVTS = juce::AudioProcessorValueTreeState;

public:
	FXComponent(APVTS& apvts);
	~FXComponent() override = default;

	void paint(juce::Graphics&) override;
	void resized() override;

private:
	void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;
	void updateVisibility();	

	// Carica un'immagine PNG embedded (icona) da BinaryData per lo sfondo quando FX = None
	juce::Image loadEmbeddedPngIcon() const;

	APVTS& apvts;

	utils::DropDown fxType;

	juce::ToggleButton bypass{ "Bypass" };
	std::unique_ptr<APVTS::ButtonAttachment>   bypassAttachment;	

	LabeledSlider wetLS;
	LabeledSlider chRateLS, chDepthLS, chDelayLS, chFeedbackLS; // Slider Chorus
	LabeledSlider flRateLS, flDepthLS, flDelayLS, flFeedbackLS; // Slider Flanger
	LabeledSlider rvSizeLS, rvDampLS, rvWidthLS; // Slider Reverb

	// Immagine di sfondo da mostrare quando l'effetto è "None"
	juce::Image bgImage;
	bool showBg{ false };
};
