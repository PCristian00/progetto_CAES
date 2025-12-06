/*
 ==============================================================================

   Utils.cpp
   Created: 8 Nov 2025 5:39:14pm
   Author:  crist

 ==============================================================================
*/

#include "Utils.h"

namespace utils
{
	/**
	 * Configura un Button (testo e cursore) e lo aggiunge al parent.
	 * Se il parent implementa Button::Listener, lo registra come listener.
	 */
	void setButton(juce::Button& button, const juce::String& buttonText, juce::Component* parent) noexcept
	{
		button.setButtonText(buttonText);
		button.setMouseCursor(juce::MouseCursor::PointingHandCursor);

		if (parent)
		{
			parent->addAndMakeVisible(button);
			if (auto* listener = dynamic_cast<juce::Button::Listener*>(parent))
				button.addListener(listener);
		}
	}

	/**
	 * Restituisce i bounds del parent ridotti dal padding (override opzionale).
	 */
	juce::Rectangle<int> getBoundsWithPadding(juce::Component* parent, int paddingOverride) noexcept
	{
		if (!parent)
			return {};

		const int usePadding = (paddingOverride == 0 ? utils::padding : paddingOverride);
		return parent->getLocalBounds().reduced(usePadding);
	}

	/**
	 * Area contenuti: bounds con padding, meno titolo, con rientro interno.
	 */
	juce::Rectangle<int> getContentArea(juce::Component* parent) noexcept
	{
		auto bounds = getBoundsWithPadding(parent);
		bounds.removeFromTop(titleAreaHeight());
		bounds.reduce(contentPadding, contentPadding);
		return bounds;
	}

	/**
	 * Disegna bordo e titolo sopra l'area dei contenuti.
	 */
	void drawBorders(juce::Graphics& g, juce::Component* parent, juce::Colour colour, juce::String title) noexcept
	{
		auto bounds = getBoundsWithPadding(parent);
		auto labelSpace = bounds.removeFromTop(titleAreaHeight());

		g.setColour(colour);
		g.setFont(15.0f);
		g.drawText(title, labelSpace.withX(padding), juce::Justification::left);
		g.drawRoundedRectangle(bounds.toFloat(),
			static_cast<float>(padding) / 2.0f,
			static_cast<float>(padding) / 5.0f);
	}

	/**
	 * Applica tema colore a una `ComboBox` e, opzionalmente, al suo `PopupMenu` tramite LookAndFeel.
	 * Se `laf` e' non nullo, imposta i colori del popup su quel `LookAndFeel`.
	 */
	void themeComboBox(juce::ComboBox& combo, juce::Colour base, juce::LookAndFeel* laf)
	{
		combo.setColour(juce::ComboBox::backgroundColourId, base.withAlpha(0.35f));
		combo.setColour(juce::ComboBox::textColourId, juce::Colours::white);
		combo.setColour(juce::ComboBox::outlineColourId, base.darker(0.5f));
		combo.setColour(juce::ComboBox::buttonColourId, base);
		combo.setColour(juce::ComboBox::arrowColourId, base.brighter(0.6f));

		if (laf)
		{
			laf->setColour(juce::PopupMenu::backgroundColourId, base.withAlpha(0.85f));
			laf->setColour(juce::PopupMenu::textColourId, juce::Colours::white);
			laf->setColour(juce::PopupMenu::highlightedBackgroundColourId, base.brighter(0.3f));
			laf->setColour(juce::PopupMenu::highlightedTextColourId, juce::Colours::black);
			laf->setColour(juce::PopupMenu::headerTextColourId, base.brighter(0.8f));
		}
	}

	/**
	 * Applica tema colore a un `Button`.
	 */
	void themeButton(juce::Button& button, juce::Colour base)
	{
		button.setColour(juce::TextButton::buttonColourId, base.withAlpha(0.25f));
		button.setColour(juce::TextButton::buttonOnColourId, base);
		button.setColour(juce::TextButton::textColourOnId, juce::Colours::white);
		button.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
	}

	/**
	 * Dispone in riga gli slider visibili, distribuendo uniformemente la larghezza.
	 *
	 * @param x,y posizione di partenza.
	 * @param totalWidth larghezza totale disponibile.
	 * @param height altezza degli slider.
	 * @param sliders lista di LabeledSlider*.
	 */
	void layoutVisibleRow(int x, int y, int totalWidth, int height, std::initializer_list<LabeledSlider*> sliders) noexcept
	{
		int visibleCount = 0;
		for (auto* ls : sliders)
			if (ls && ls->slider.isVisible())
				++visibleCount;

		if (visibleCount == 0)
			return;

		const int availableWidth = totalWidth - padding * (visibleCount - 1);
		const int columnWidth = availableWidth / visibleCount;

		int nextX = x;
		for (auto* ls : sliders)
		{
			if (ls && ls->slider.isVisible())
			{
				ls->setBounds(nextX, y, columnWidth, height, padding * 2, 0);
				nextX += columnWidth + padding;
			}
		}
	}

	/**
	 * Dispone una DropDown e, sotto, una riga di slider visibili.
	 * Se dimensioni non specificate, calcola da content area.
	 */
	void comboAndSliderRow(utils::DropDown& dd, std::initializer_list<LabeledSlider*> sliders, juce::Component* parent, int x, int y, int totalWidth, int height) noexcept
	{
		auto content = getContentArea(parent);

		if (height == 0)
			height = content.getHeight() / 6;
		if (totalWidth == 0)
			totalWidth = content.getWidth();

		if (x == 0)
			x = content.getX();
		if (y == 0)
			y = content.getY();

		dd.setBounds(x, y, totalWidth, height);

		const int rowY = dd.cBox.getBottom() + padding;
		const int rowH = content.getBottom() - rowY; // tutto lo spazio rimanente

		layoutVisibleRow(x, rowY, totalWidth, rowH, sliders);
	}
}