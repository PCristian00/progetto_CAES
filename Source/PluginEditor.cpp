/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SubSynthAudioProcessorEditor::SubSynthAudioProcessorEditor(SubSynthAudioProcessor& p)
	: AudioProcessorEditor(&p), audioProcessor(p)
{
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.

	setSize(400, 300);

	setSliderParams(gainSlider);

	setSliderParams(attackSlider);
	setSliderParams(decaySlider);
	setSliderParams(sustainSlider);
	setSliderParams(releaseSlider);

	gainSliderAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "GAIN", gainSlider);

	attackSliderAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "ATTACK", attackSlider);
	decaySliderAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "DECAY", decaySlider);
	sustainSliderAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "SUSTAIN", sustainSlider);
	releaseSliderAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "RELEASE", releaseSlider);

	oscSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "OSC", oscSelector);


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
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..

	const auto bounds = getLocalBounds().reduced(10);
	const auto padding = 10;
	const auto sliderWidth = bounds.getWidth() / 4 - padding;
	const auto sliderHeight = bounds.getHeight() / 4 - padding;
	const auto sliderXstart = 0;
	const auto sliderYstart = bounds.getHeight() / 2 - (sliderHeight / 2);

	setSliderBounds(attackSlider, sliderXstart, sliderYstart, sliderWidth, sliderHeight);
	setSliderBounds(decaySlider, attackSlider.getRight() + padding, sliderYstart, sliderWidth, sliderHeight);
	setSliderBounds(sustainSlider, decaySlider.getRight() + padding, sliderYstart, sliderWidth, sliderHeight);
	setSliderBounds(releaseSlider, sustainSlider.getRight() + padding, sliderYstart, sliderWidth, sliderHeight);

	setSliderBounds(gainSlider, releaseSlider.getRight() + padding, sliderYstart, sliderWidth, sliderHeight);
}

void SubSynthAudioProcessorEditor::setSliderParams(juce::Slider& slider) {
	slider.setSliderStyle(juce::Slider::LinearBarVertical);
	slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
	addAndMakeVisible(slider);
}

void SubSynthAudioProcessorEditor::setSliderBounds(juce::Slider& slider, int x, int y, int width, int height) {
	slider.setBounds(x, y, width, height);
}
