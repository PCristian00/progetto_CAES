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


DialogBox::DialogBox(juce::String messageText, juce::String acceptButtonText, juce::String closeButtonText, std::function<void()>& acceptFunction)
{
	message.setText(messageText, juce::dontSendNotification);
	addAndMakeVisible(message);

	if (acceptButtonText.isNotEmpty())
		configureButton(leftButton, acceptButtonText);
	if (closeButtonText.isNotEmpty())
		configureButton(rightButton, closeButtonText);

	this->leftFunction = acceptFunction;
	this->rightFunction = closeFunction;
}

DialogBox::DialogBox(juce::String messageText, juce::String leftButtonText, juce::String rightButtonText, std::function<void()>& leftFunction, std::function<void()>& rightFunction)
{
	new DialogBox(messageText, leftButtonText, rightButtonText, leftFunction);
	this->rightFunction = rightFunction;
}

DialogBox::~DialogBox()
{
	leftButton.removeListener(this);
	rightButton.removeListener(this);
}


// CAPIRE COME RESTITUIRE QUALE BUTTON E' STATO CLICCATO ALL'ESTERNO
void DialogBox::buttonClicked(juce::Button* button) {

	if (button == &leftButton) {
		// Handle left button click
		leftFunction();
	}
	else if (button == &rightButton) {
		// Handle right button click
		rightFunction();
	}
}

void DialogBox::paint(juce::Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background
}

void DialogBox::resized()
{
	// This method is where you should set the bounds of any child
	// components that your component contains..
	const auto container = getLocalBounds().reduced(4);
	// const auto container = utils::getBoundsWithPadding(this, 4);
	auto bounds = container;


	message.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.6f)).reduced(4));
	setButtonBounds(rightButton, bounds.removeFromRight(container.proportionOfWidth(0.2f)).reduced(4));
	setButtonBounds(leftButton, bounds.removeFromRight(container.proportionOfWidth(0.2f)).reduced(4));


}

// da migliorare, spostare in utils o rimuovere

void DialogBox::configureButton(juce::Button& button, const juce::String& buttonText) {
	button.setButtonText(buttonText);
	button.setMouseCursor(juce::MouseCursor::PointingHandCursor);
	addAndMakeVisible(button);
	button.addListener(this);
}

void DialogBox::setButtonBounds(juce::Button& button, juce::Rectangle<int> size) {
	button.setBounds(size);
}