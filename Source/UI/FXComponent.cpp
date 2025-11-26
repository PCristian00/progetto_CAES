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

FXComponent::FXComponent(juce::AudioProcessorValueTreeState& state) : apvts(state)
{
	utils::setComboBoxParams(fxType, fxTypeAttachment, apvts, parameters::FX_TYPE, { "None", "Chorus", "Flanger", "Reverb" }, this);
	fxType.addListener(this);

	// Assicura che updateVisibility scatti anche quando la Attachment cambia il valore (es. al restore)
	fxType.onChange = [this]
		{
			updateVisibility();
			resized(); // ridisegna il layout coerente con la visibilità
		};

	addAndMakeVisible(bypass);
	bypassAttachment = std::make_unique<APVTS::ButtonAttachment>(apvts, parameters::FX_BYPASS, bypass);

	// Sliders
	utils::setSliderParams(wetLS.slider, wetLS.attachment, apvts, parameters::FX_WET, wetLS.label, this, juce::Slider::SliderStyle::LinearBar);

	utils::setSliderParams(chRateLS.slider, chRateLS.attachment, apvts, parameters::CH_RATE, chRateLS.label, this, juce::Slider::SliderStyle::LinearBarVertical);
	utils::setSliderParams(chDepthLS.slider, chDepthLS.attachment, apvts, parameters::CH_DEPTH, chDepthLS.label, this, juce::Slider::SliderStyle::LinearBarVertical);
	utils::setSliderParams(chDelayLS.slider, chDelayLS.attachment, apvts, parameters::CH_DELAY_MS, chDelayLS.label, this, juce::Slider::SliderStyle::LinearBarVertical);
	utils::setSliderParams(chFeedbackLS.slider, chFeedbackLS.attachment, apvts, parameters::CH_FEEDBACK, chFeedbackLS.label, this, juce::Slider::SliderStyle::LinearBarVertical);

	utils::setSliderParams(flRateLS.slider, flRateLS.attachment, apvts, parameters::FL_RATE, flRateLS.label, this, juce::Slider::SliderStyle::LinearBarVertical);
	utils::setSliderParams(flDepthLS.slider, flDepthLS.attachment, apvts, parameters::FL_DEPTH, flDepthLS.label, this, juce::Slider::SliderStyle::LinearBarVertical);
	utils::setSliderParams(flDelayLS.slider, flDelayLS.attachment, apvts, parameters::FL_DELAY_MS, flDelayLS.label, this, juce::Slider::SliderStyle::LinearBarVertical);
	utils::setSliderParams(flFeedbackLS.slider, flFeedbackLS.attachment, apvts, parameters::FL_FEEDBACK, flFeedbackLS.label, this, juce::Slider::SliderStyle::LinearBarVertical);

	utils::setSliderParams(rvSizeLS.slider, rvSizeLS.attachment, apvts, parameters::RV_SIZE, rvSizeLS.label, this, juce::Slider::SliderStyle::LinearBarVertical);
	utils::setSliderParams(rvDampLS.slider, rvDampLS.attachment, apvts, parameters::RV_DAMP, rvDampLS.label, this, juce::Slider::SliderStyle::LinearBarVertical);
	utils::setSliderParams(rvWidthLS.slider, rvWidthLS.attachment, apvts, parameters::RV_WIDTH, rvWidthLS.label, this, juce::Slider::SliderStyle::LinearBarVertical);

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

// Abbastanza generica per layout elastico di una riga di sliders etichettati, spostare in utils
void FXComponent::layoutVisibleRow(int x, int y, int totalWidth, int height, std::initializer_list<LabeledSlider*> sliders)
{
	// Conta quanti sono visibili
	int visibleCount = 0;
	for (auto* ls : sliders)
		if (ls && ls->slider.isVisible())
			++visibleCount;

	if (visibleCount == 0)
		return;

	// Larghezza per colonna, con padding tra colonne
	const int padding = utils::padding;
	const int availableWidth = totalWidth - padding * (visibleCount - 1);
	const int columnWidth = availableWidth / visibleCount;

	int nextX = x;
	for (auto* ls : sliders)
	{
		if (ls && ls->slider.isVisible())
		{
			utils::setSliderBounds(ls->slider, nextX, y, columnWidth, height, ls->label);
			nextX += columnWidth + padding;
		}
	}
}

void FXComponent::paint(juce::Graphics& g)
{
	g.fillAll(juce::Colours::black);
	utils::drawBorders(g, this, juce::Colours::deeppink, "FX");
}

void FXComponent::resized()
{
	const int heightUnit = utils::getBoundsWithPadding(this).getHeight() / 3 - utils::padding;
	const int widthUnit = utils::getBoundsWithPadding(this).getWidth() / 4 - utils::padding;
	const int totalWidth = utils::getBoundsWithPadding(this).getWidth() - utils::padding;

	const int comboBoxWidth = (utils::getBoundsWithPadding(this).getWidth() - utils::padding) / 3;
	const int comboBoxHeight = heightUnit - utils::padding;

	utils::setComboBoxBounds(fxType, utils::Xstart, utils::Ystart, comboBoxWidth, comboBoxHeight);
	bypass.setBounds(fxType.getRight() + utils::padding, fxType.getY(), comboBoxWidth - utils::padding, comboBoxHeight);

	utils::setSliderBounds(wetLS.slider, bypass.getRight() + utils::padding, bypass.getY(), comboBoxWidth - utils::padding, comboBoxHeight, wetLS.label);

	const int rowY = fxType.getBottom() + utils::padding;
	const int rowH = 2 * heightUnit;

	// Layout elastico per ogni gruppo

	auto chSliders = { &chRateLS, &chDepthLS, &chDelayLS, &chFeedbackLS };
	auto flSliders = { &flRateLS, &flDepthLS, &flDelayLS, &flFeedbackLS };
	auto rvSliders = { &rvSizeLS, &rvDampLS, &rvWidthLS };

	layoutVisibleRow(utils::Xstart, rowY, totalWidth, rowH, chSliders);
	layoutVisibleRow(utils::Xstart, rowY, totalWidth, rowH, flSliders);
	layoutVisibleRow(utils::Xstart, rowY, totalWidth, rowH, rvSliders);
}
