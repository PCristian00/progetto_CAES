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

	leftFunction = std::move(acceptFunction);
	rightFunction = std::move(closeFunction);
}

DialogBox::DialogBox(juce::String messageText, juce::String leftButtonText, juce::String rightButtonText, std::function<void()>& leftFunction, std::function<void()>& rightFunction) : DialogBox(messageText, leftButtonText, rightButtonText, leftFunction)
{
	this->rightFunction = std::move(rightFunction);
}

DialogBox::~DialogBox()
{
	leftButton.removeListener(this);
	rightButton.removeListener(this);
}


// CAPIRE COME RESTITUIRE QUALE BUTTON E' STATO CLICCATO ALL'ESTERNO
void DialogBox::buttonClicked(juce::Button* button) {

	if (button == &leftButton && leftFunction) {
		// Handle left button click
		leftFunction();
	}
	else if (button == &rightButton && rightFunction) {
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
	const auto container = getLocalBounds().reduced(4);
	auto bounds = container;

	auto buttonWidth = container.proportionOfWidth(0.2f);

	if (rightButton.isVisible() == false || leftButton.isVisible() == false)
	{
		buttonWidth = buttonWidth * 2;
	}

	message.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.6f)).reduced(4));

	if (rightButton.isVisible())
		setButtonBounds(rightButton, bounds.removeFromRight(buttonWidth).reduced(4));
	if (leftButton.isVisible())
		setButtonBounds(leftButton, bounds.removeFromRight(buttonWidth).reduced(4));
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