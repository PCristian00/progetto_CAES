/*
  ==============================================================================

	DialogBox.cpp
	Created: 20 Nov 2025 7:00:37pm
	Author:  crist

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DialogBox.h"
#include "Utils.h"

//==============================================================================


DialogBox::DialogBox(juce::String messageText, juce::String acceptButtonText, juce::String closeButtonText, std::function<void()>& acceptFunction)
{
	message.setText(messageText, juce::dontSendNotification);
	addAndMakeVisible(message);

	if (acceptButtonText.isNotEmpty())
		utils::setButton(leftButton, acceptButtonText, this);
	if (closeButtonText.isNotEmpty())
		utils::setButton(rightButton, closeButtonText, this);

	leftFunction = std::move(acceptFunction);
	rightFunction = [this]() { this->close(); };
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


void DialogBox::paint(juce::Graphics& g)
{
	g.fillAll(juce::Colours::black);
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

	if (rightButton.isVisible()) {
		rightButton.setBounds(bounds.removeFromRight(buttonWidth).reduced(4));
		// utils::setButtonBounds(rightButton, bounds.removeFromRight(buttonWidth).reduced(4));
	}
	if (leftButton.isVisible()) {
		leftButton.setBounds(bounds.removeFromRight(buttonWidth).reduced(4));
		// utils::setButtonBounds(leftButton, bounds.removeFromRight(buttonWidth).reduced(4));
	}
}

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

void DialogBox::show() {
	this->setVisible(true);
	this->toFront(true);
}

void DialogBox::close() {
	this->setVisible(false);
}