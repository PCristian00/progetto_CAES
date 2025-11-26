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
	const int heightUnit = utils::getBoundsWithPadding(this).getHeight() / 3 - utils::padding;
	const int totalWidth = utils::getBoundsWithPadding(this).getWidth() - utils::padding;
	const int comboBoxWidth = (utils::getBoundsWithPadding(this).getWidth() - utils::padding) / 3;
	const int comboBoxHeight = heightUnit - utils::padding;

	utils::setComboBoxBounds(fxType, utils::Xstart, utils::Ystart, comboBoxWidth, comboBoxHeight);
	bypass.setBounds(fxType.getRight() + utils::padding, fxType.getY(), comboBoxWidth - utils::padding, comboBoxHeight);

	wetLS.setBounds(bypass.getRight() + utils::padding, bypass.getY(), comboBoxWidth - utils::padding, comboBoxHeight);

	const int rowY = fxType.getBottom() + utils::padding;
	const int rowH = 2 * heightUnit;

	auto chSliders = { &chRateLS, &chDepthLS, &chDelayLS, &chFeedbackLS };
	auto flSliders = { &flRateLS, &flDepthLS, &flDelayLS, &flFeedbackLS };
	auto rvSliders = { &rvSizeLS, &rvDampLS, &rvWidthLS };

	utils::layoutVisibleRow(utils::Xstart, rowY, totalWidth, rowH, chSliders);
	utils::layoutVisibleRow(utils::Xstart, rowY, totalWidth, rowH, flSliders);
	utils::layoutVisibleRow(utils::Xstart, rowY, totalWidth, rowH, rvSliders);
}
