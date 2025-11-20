/*
  ==============================================================================

	DialogBox.cpp
	Created: 20 Nov 2025 7:00:37pm
	Author:  crist

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DialogBox.h"

//==============================================================================
DialogBox::DialogBox(juce::String message)
{
	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.

	configureButton(confirmButton, "Confirm");
	configureButton(returnButton, "Return");

}

DialogBox::~DialogBox()
{
	confirmButton.removeListener(this);
	returnButton.removeListener(this);
}

// CAPIRE COME RESTITUIRE QUALE BUTTON E' STATO CLICCATO ALL'ESTERNO
void DialogBox::buttonClicked(juce::Button* button) {
	if (button == &confirmButton) {
		// Handle confirm button click

	}
	else if (button == &returnButton) {
		// Handle return button click
	}
}

void DialogBox::paint(juce::Graphics& g)
{
	/* This demo code just fills the component's background and
	   draws some placeholder text to get you started.

	   You should replace everything in this method with your own
	   drawing code..
	*/

	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

	g.setColour(juce::Colours::grey);
	g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

	g.setColour(juce::Colours::white);
	g.setFont(juce::FontOptions(14.0f));
	g.drawText(message, getLocalBounds(),
		juce::Justification::centred, true);   // draw some placeholder text
}

void DialogBox::resized()
{
	// This method is where you should set the bounds of any child
	// components that your component contains..
	const auto container = getLocalBounds().reduced(4);
	// const auto container = utils::getBoundsWithPadding(this, 4);
	auto bounds = container;

	// juce::Rectangle<int>

	// auto buttonSize = (bounds.removeFromLeft(container.proportionOfWidth(0.2f)).reduced(4));

	// Rendere migliori queste funzioni

	setButtonBounds(confirmButton, bounds.removeFromLeft(container.proportionOfWidth(0.2f)).reduced(4));
	setButtonBounds(returnButton, bounds.removeFromLeft(container.proportionOfWidth(0.1f)).reduced(4));

}

void DialogBox::configureButton(juce::Button& button, const juce::String& buttonText) {
	button.setButtonText(buttonText);
	button.setMouseCursor(juce::MouseCursor::PointingHandCursor);
	addAndMakeVisible(button);
	button.addListener(this);
}

// da migliorare, spostare in utils o rimuovere
void DialogBox::setButtonBounds(juce::Button& button, juce::Rectangle<int> size) {
	button.setBounds(size);
}