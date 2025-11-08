/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SubSynthAudioProcessorEditor::SubSynthAudioProcessorEditor(SubSynthAudioProcessor& p)
	: AudioProcessorEditor(&p), audioProcessor(p), adsr(audioProcessor.apvts)
{
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.

	setSize(800, 600);

	setSliderParams(gainSlider, this);

	gainSliderAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "GAIN", gainSlider);

	oscSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "OSC", oscSelector);

	addAndMakeVisible(adsr);

}

SubSynthAudioProcessorEditor::~SubSynthAudioProcessorEditor()
{
}

//==============================================================================
void SubSynthAudioProcessorEditor::paint(juce::Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

	g.setColour(juce::Colours::white);
	g.setFont(juce::FontOptions(15.0f));
	// g.drawFittedText("Sub Synth", getLocalBounds(), juce::Justification::centred, 1);
}

void SubSynthAudioProcessorEditor::resized()
{

	adsr.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight());
	setSliderBounds(gainSlider, 20, 10, 100, 200);
	
	// SPOSTATO IN ADSRComponent.cpp
	// 
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..

	/*const auto bounds = getLocalBounds().reduced(10);
	const auto padding = 10;
	const int numSliders = 5;
	const auto sliderWidth = bounds.getWidth() / numSliders - padding;
	const auto sliderHeight = bounds.getHeight() / numSliders - padding;
	const auto sliderXstart = padding;
	const auto sliderYstart = bounds.getHeight() / 2 - (sliderHeight / 2);

	setSliderBounds(attackSlider, sliderXstart, sliderYstart, sliderWidth, sliderHeight);
	setSliderBounds(decaySlider, attackSlider.getRight() + padding, sliderYstart, sliderWidth, sliderHeight);
	setSliderBounds(sustainSlider, decaySlider.getRight() + padding, sliderYstart, sliderWidth, sliderHeight);
	setSliderBounds(releaseSlider, sustainSlider.getRight() + padding, sliderYstart, sliderWidth, sliderHeight);*/

	// ATTENZIONE: Slider del gain temporaneamente disattivato, forse CREARE COMPONENTE anche per Gain come per ADSR
	// setSliderBounds(gainSlider, releaseSlider.getRight() + padding, sliderYstart, sliderWidth, sliderHeight);
}
