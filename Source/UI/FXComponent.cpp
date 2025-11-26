/*
  ==============================================================================

	FXComponent.cpp
	Created: 25 Nov 2025 12:49:57pm
	Author:  crist

  ==============================================================================
*/

#include "FXComponent.h"
#include "../Parameters.h"
#include "Utils.h"

FXComponent::FXComponent(juce::AudioProcessorValueTreeState& state)
	: apvts(state)
	, wetLS("Wet", apvts, parameters::FX_WET, *this, juce::Slider::LinearBar, true)
	, chRateLS("Ch Rate", apvts, parameters::CH_RATE, *this, juce::Slider::LinearBarVertical, true)
	, chDepthLS("Ch Depth", apvts, parameters::CH_DEPTH, *this, juce::Slider::LinearBarVertical, true)
	, chDelayLS("Ch Delay", apvts, parameters::CH_DELAY_MS, *this, juce::Slider::LinearBarVertical, true)
	, chFeedbackLS("Ch Feedback", apvts, parameters::CH_FEEDBACK, *this, juce::Slider::LinearBarVertical, true)
	, flRateLS("Fl Rate", apvts, parameters::FL_RATE, *this, juce::Slider::LinearBarVertical, true)
	, flDepthLS("Fl Depth", apvts, parameters::FL_DEPTH, *this, juce::Slider::LinearBarVertical, true)
	, flDelayLS("Fl Delay", apvts, parameters::FL_DELAY_MS, *this, juce::Slider::LinearBarVertical, true)
	, flFeedbackLS("Fl Feedback", apvts, parameters::FL_FEEDBACK, *this, juce::Slider::LinearBarVertical, true)
	, rvSizeLS("Rev Size", apvts, parameters::RV_SIZE, *this, juce::Slider::LinearBarVertical, true)
	, rvDampLS("Rev Damp", apvts, parameters::RV_DAMP, *this, juce::Slider::LinearBarVertical, true)
	, rvWidthLS("Rev Width", apvts, parameters::RV_WIDTH, *this, juce::Slider::LinearBarVertical, true)
{
	utils::setComboBoxParams(fxType, fxTypeAttachment, apvts, parameters::FX_TYPE,
		{ "None", "Chorus", "Flanger", "Reverb" }, this);
	fxType.addListener(this);

	fxType.onChange = [this]
	{
		updateVisibility();
		resized();
	};

	addAndMakeVisible(bypass);
	bypassAttachment = std::make_unique<APVTS::ButtonAttachment>(apvts, parameters::FX_BYPASS, bypass);

	updateVisibility();
}

void FXComponent::comboBoxChanged(juce::ComboBox* c)
{
	if (c == &fxType)
		updateVisibility();
}

void FXComponent::updateVisibility()
{
	const int type = fxType.getSelectedId() - 1; // 0=None,1=Chorus,2=Flanger,3=Reverb
	const bool showCh = (type == 1);
	const bool showFl = (type == 2);
	const bool showRv = (type == 3);

	chRateLS.setVisible(showCh); chDepthLS.setVisible(showCh); chDelayLS.setVisible(showCh); chFeedbackLS.setVisible(showCh);
	flRateLS.setVisible(showFl); flDepthLS.setVisible(showFl); flDelayLS.setVisible(showFl); flFeedbackLS.setVisible(showFl);
	rvSizeLS.setVisible(showRv); rvDampLS.setVisible(showRv); rvWidthLS.setVisible(showRv);
}

void FXComponent::paint(juce::Graphics& g)
{
	g.fillAll(juce::Colours::black);
	utils::drawBorders(g, this, juce::Colours::deeppink, "FX");
}

void FXComponent::resized()
{
	// Area contenuti uniforme (niente moltiplicazioni manuali di padding)
	const auto content = utils::getContentArea(this);
	const int totalWidth = content.getWidth();
	const int startX = content.getX();
	const int startY = content.getY();

	// Riga superiore: 3 colonne con gutter = utils::padding
	const int gaps = 2;
	const int colW = (totalWidth - gaps * utils::padding) / 3;
	const int headerH = content.getHeight() / 6;

	// Colonne
	const int x0 = startX;
	const int x1 = x0 + colW + utils::padding;
	const int x2 = x1 + colW + utils::padding;

	// Posizionamento controlli di testata
	utils::setComboBoxBounds(fxType, x0, startY, colW, headerH);
	bypass.setBounds(x1, startY, colW, headerH);
	wetLS.setBounds(x2, startY, colW, headerH);

	// Riga slider: occupa lo spazio restante sotto la testata
	const int rowY = startY + headerH + utils::padding;
	const int rowH = content.getBottom() - rowY;

	auto chSliders = { &chRateLS, &chDepthLS, &chDelayLS, &chFeedbackLS };
	auto flSliders = { &flRateLS, &flDepthLS, &flDelayLS, &flFeedbackLS };
	auto rvSliders = { &rvSizeLS, &rvDampLS, &rvWidthLS };

	utils::layoutVisibleRow(startX, rowY, totalWidth, rowH, chSliders);
	utils::layoutVisibleRow(startX, rowY, totalWidth, rowH, flSliders);
	utils::layoutVisibleRow(startX, rowY, totalWidth, rowH, rvSliders);
}
