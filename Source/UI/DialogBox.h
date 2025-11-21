/*
  ==============================================================================

	DialogBox.h
	Created: 20 Nov 2025 7:00:37pm
	Author:  crist

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class DialogBox : public juce::Component, public juce::Button::Listener
{
public:
	
	DialogBox(juce::String messageText, juce::String acceptButtonText, juce::String closeButtonText, std::function<void()>& acceptFunction);
	DialogBox(juce::String messageText, juce::String leftButtonText, juce::String rightButtonText, std::function<void()>& leftFunction, std::function<void()>& rightFunction);

	~DialogBox() override;

	void paint(juce::Graphics&) override;
	void resized() override;

	void configureButton(juce::Button& button, const juce::String& buttonText);

	void setButtonBounds(juce::Button& button, juce::Rectangle<int> size);

	void buttonClicked(juce::Button* button) override;
private:

	juce::Label message;
	juce::TextButton rightButton, leftButton;

	std::function <void()> leftFunction, rightFunction;

	std::function <void()> closeFunction = [this]() {
		this->setVisible(false);
		};

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DialogBox)
};
