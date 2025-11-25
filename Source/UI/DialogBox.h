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

//==============================================================================
// Semplice dialog con uno o due pulsanti.
// Costruttore 1: messaggio + testo bottone sinistro (accept) + testo bottone destro (close) + callback sinistro.
//                Il destro chiude la dialog (se presente).
// Costruttore 2: messaggio + testi dei due bottoni + due callback separate.
//
class DialogBox : public juce::Component, public juce::Button::Listener
{
public:

	DialogBox(juce::String messageText, juce::String acceptButtonText, juce::String closeButtonText, std::function<void()>& acceptFunction);
	DialogBox(juce::String messageText, juce::String leftButtonText, juce::String rightButtonText, std::function<void()>& leftFunction, std::function<void()>& rightFunction);

	~DialogBox() override;

	void paint(juce::Graphics&) override;
	void resized() override;

	void buttonClicked(juce::Button* button) override;

	void show();
	void close();

private:

	juce::Label message;
	juce::TextButton rightButton, leftButton;

	std::function <void()> leftFunction, rightFunction;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DialogBox)
};
