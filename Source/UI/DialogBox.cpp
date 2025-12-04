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


/**
 * Costruttore base: crea dialog con messaggio e fino a due pulsanti.
 * Il pulsante di sinistra esegue la funzione passata; il destro chiude il dialog.
 *
 * @param messageText testo del messaggio.
 * @param acceptButtonText testo pulsante sinistro (vuoto = nascosto).
 * @param closeButtonText testo pulsante destro (vuoto = nascosto).
 * @param acceptFunction funzione eseguita al click pulsante sinistro.
 */
DialogBox::DialogBox(juce::String messageText, juce::String acceptButtonText, juce::String closeButtonText, std::function<void()>& acceptFunction)
{
	message.setText(messageText, juce::dontSendNotification);
	addAndMakeVisible(message);

	if (acceptButtonText.isNotEmpty()) {
		utils::setButton(leftButton, acceptButtonText, this);
		utils::themeButton(leftButton, utils::presetCol);

	}
	if (closeButtonText.isNotEmpty()) {
		utils::setButton(rightButton, closeButtonText, this);
		utils::themeButton(rightButton, utils::presetCol);
	}

	leftFunction = std::move(acceptFunction);
	rightFunction = [this]() { this->close(); };
}

/**
 * Costruttore esteso: permette di specificare due funzioni diverse per i pulsanti.
 *
 * @param messageText testo del messaggio.
 * @param leftButtonText testo pulsante sinistro.
 * @param rightButtonText testo pulsante destro.
 * @param leftFunction azione sul pulsante sinistro.
 * @param rightFunction azione sul pulsante destro.
 */
DialogBox::DialogBox(juce::String messageText, juce::String leftButtonText, juce::String rightButtonText, std::function<void()>& leftFunction, std::function<void()>& rightFunction) : DialogBox(messageText, leftButtonText, rightButtonText, leftFunction)
{
	this->rightFunction = std::move(rightFunction);
}

/**
 * Distruttore: rimuove i listener e chiude eventuale stato.
 */
DialogBox::~DialogBox()
{
	leftButton.removeListener(this);
	rightButton.removeListener(this);
}


/**
 * Disegna lo sfondo del dialog (colore uniforme).
 */
void DialogBox::paint(juce::Graphics& g)
{
	g.fillAll(juce::Colours::black);
}

/**
 * Layout: dimensiona messaggio e pulsanti; se presente solo un pulsante ne raddoppia la larghezza.
 */
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
	}
	if (leftButton.isVisible()) {
		leftButton.setBounds(bounds.removeFromRight(buttonWidth).reduced(4));
	}
}

/**
 * Gestione click: esegue funzione associata al pulsante premuto.
 */
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

/**
 * Mostra il dialog portandolo in primo piano.
 */
void DialogBox::show() {
	this->setVisible(true);
	this->toFront(true);
}

/**
 * Nasconde il dialog.
 */
void DialogBox::close() {
	this->setVisible(false);
}