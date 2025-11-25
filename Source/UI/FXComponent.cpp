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

//static void styleSlider(juce::Slider& s, const juce::String& name)
//{
//	s.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 18);
//	s.setName(name);
//}

FXComponent::FXComponent(juce::AudioProcessorValueTreeState& state) : apvts(state)
{
	// FX Type

	/*fxType.addItem("None", 1);
	fxType.addItem("Chorus", 2);
	fxType.addItem("Flanger", 3);
	fxType.addItem("Reverb", 4);
	addAndMakeVisible(fxType);*/


	utils::setComboBoxParams(fxType, fxTypeAttachment, apvts, parameters::FX_TYPE, { "None", "Chorus", "Flanger", "Reverb" }, this);
	fxType.addListener(this);

	// Inserire nuova funzione di utils per la gestione dei toggle button
	addAndMakeVisible(bypass);
	bypassAttachment = std::make_unique<APVTS::ButtonAttachment>(apvts, parameters::FX_BYPASS, bypass);


	// Sliders
	// styleSlider(wet, "Wet");
	utils::setSliderParams(wet, wetAttachment, apvts, parameters::FX_WET, wetLabel, this, juce::Slider::SliderStyle::LinearBar);

	utils::setSliderParams(chRate, chRateAttachment, apvts, parameters::CH_RATE, chRateLabel, this, juce::Slider::SliderStyle::LinearBarVertical);
	utils::setSliderParams(chDepth, chDepthAttachment, apvts, parameters::CH_DEPTH, chDepthLabel, this, juce::Slider::SliderStyle::LinearBarVertical);
	utils::setSliderParams(chDelay, chDelayAttachment, apvts, parameters::CH_DELAY_MS, chDelayLabel, this, juce::Slider::SliderStyle::LinearBarVertical);
	utils::setSliderParams(chFeedback, chFeedbackAttachment, apvts, parameters::CH_FEEDBACK, chFeedbackLabel, this, juce::Slider::SliderStyle::LinearBarVertical);
	utils::setSliderParams(flRate, flRateAttachment, apvts, parameters::FL_RATE, flRateLabel, this, juce::Slider::SliderStyle::LinearBarVertical);
	utils::setSliderParams(flDepth, flDepthAttachment, apvts, parameters::FL_DEPTH, flDepthLabel, this, juce::Slider::SliderStyle::LinearBarVertical);
	utils::setSliderParams(flDelay, flDelayAttachment, apvts, parameters::FL_DELAY_MS, flDelayLabel, this, juce::Slider::SliderStyle::LinearBarVertical);
	utils::setSliderParams(flFeedback, flFeedbackAttachment, apvts, parameters::FL_FEEDBACK, flFeedbackLabel, this, juce::Slider::SliderStyle::LinearBarVertical);
	utils::setSliderParams(rvSize, rvSizeAttachment, apvts, parameters::RV_SIZE, rvSizeLabel, this, juce::Slider::SliderStyle::LinearBarVertical);
	utils::setSliderParams(rvDamp, rvDampAttachment, apvts, parameters::RV_DAMP, rvDampLabel, this, juce::Slider::SliderStyle::LinearBarVertical);
	utils::setSliderParams(rvWidth, rvWidthAttachment, apvts, parameters::RV_WIDTH, rvWidthLabel, this, juce::Slider::SliderStyle::LinearBarVertical);

	//// styleSlider(chRate, "Ch Rate");
	//styleSlider(chDepth, "Ch Depth");
	//styleSlider(chDelay, "Ch Delay");
	//styleSlider(chFeedback, "Ch Feedback");

	//styleSlider(flRate, "Fl Rate");
	//styleSlider(flDepth, "Fl Depth");
	//styleSlider(flDelay, "Fl Delay");
	//styleSlider(flFeedback, "Fl Feedback");

	//styleSlider(rvSize, "Rev Size");
	//styleSlider(rvDamp, "Rev Damp");
	//styleSlider(rvWidth, "Rev Width");

	/*addAndMakeVisible(wet);
	addAndMakeVisible(chRate); addAndMakeVisible(chDepth); addAndMakeVisible(chDelay); addAndMakeVisible(chFeedback);
	addAndMakeVisible(flRate); addAndMakeVisible(flDepth); addAndMakeVisible(flDelay); addAndMakeVisible(flFeedback);
	addAndMakeVisible(rvSize); addAndMakeVisible(rvDamp); addAndMakeVisible(rvWidth);*/

	//using namespace parameters;
	// fxTypeAtt = std::make_unique<APVTS::ComboBoxAttachment>(apvts, FX_TYPE, fxType);

	// wetAtt = std::make_unique<APVTS::SliderAttachment>(apvts, FX_WET, wet);

	/*chRateAtt = std::make_unique<APVTS::SliderAttachment>(apvts, CH_RATE, chRate);
	chDepthAtt = std::make_unique<APVTS::SliderAttachment>(apvts, CH_DEPTH, chDepth);
	chDelayAtt = std::make_unique<APVTS::SliderAttachment>(apvts, CH_DELAY_MS, chDelay);
	chFeedbackAtt = std::make_unique<APVTS::SliderAttachment>(apvts, CH_FEEDBACK, chFeedback);

	flRateAtt = std::make_unique<APVTS::SliderAttachment>(apvts, FL_RATE, flRate);
	flDepthAtt = std::make_unique<APVTS::SliderAttachment>(apvts, FL_DEPTH, flDepth);
	flDelayAtt = std::make_unique<APVTS::SliderAttachment>(apvts, FL_DELAY_MS, flDelay);
	flFeedbackAtt = std::make_unique<APVTS::SliderAttachment>(apvts, FL_FEEDBACK, flFeedback);

	rvSizeAtt = std::make_unique<APVTS::SliderAttachment>(apvts, RV_SIZE, rvSize);
	rvDampAtt = std::make_unique<APVTS::SliderAttachment>(apvts, RV_DAMP, rvDamp);
	rvWidthAtt = std::make_unique<APVTS::SliderAttachment>(apvts, RV_WIDTH, rvWidth);*/

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

	chRate.setVisible(showCh); chDepth.setVisible(showCh); chDelay.setVisible(showCh); chFeedback.setVisible(showCh);
	chRateLabel.setVisible(showCh); chDepthLabel.setVisible(showCh); chDelayLabel.setVisible(showCh); chFeedbackLabel.setVisible(showCh);
	flRate.setVisible(showFl); flDepth.setVisible(showFl); flDelay.setVisible(showFl); flFeedback.setVisible(showFl);
	flRateLabel.setVisible(showFl); flDepthLabel.setVisible(showFl); flDelayLabel.setVisible(showFl); flFeedbackLabel.setVisible(showFl);
	rvSize.setVisible(showRv); rvDamp.setVisible(showRv); rvWidth.setVisible(showRv);
	rvSizeLabel.setVisible(showRv); rvDampLabel.setVisible(showRv); rvWidthLabel.setVisible(showRv);
}

void FXComponent::showFXSliders(int x, int y, int width, int height, juce::Slider& a, juce::Slider& b, juce::Slider& c, juce::Slider& d)
{
	utils::setSliderBounds(a, utils::Xstart, fxType.getBottom() + utils::padding, width, height, chRateLabel);
	utils::setSliderBounds(chDepth, chRate.getRight() + utils::padding, chRate.getY(), width, height, chDepthLabel);
	utils::setSliderBounds(chDelay, chDepth.getRight() + utils::padding, chRate.getY(), width, height, chDelayLabel);
	utils::setSliderBounds(chFeedback, chDelay.getRight() + utils::padding, chRate.getY(), width, height, chFeedbackLabel);
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

	const int comboBoxWidth = (utils::getBoundsWithPadding(this).getWidth() - utils::padding) / 3;
	const int comboBoxHeight = heightUnit - utils::padding;

	utils::setComboBoxBounds(fxType, utils::Xstart, utils::Ystart, comboBoxWidth, comboBoxHeight);
	bypass.setBounds(fxType.getRight() + utils::padding, fxType.getY(), comboBoxWidth - utils::padding, comboBoxHeight);
	utils::setSliderBounds(wet, bypass.getRight() + utils::padding, bypass.getY(), comboBoxWidth - utils::padding, comboBoxHeight, wetLabel);


	showFXSliders(utils::Xstart, fxType.getBottom() + utils::padding, widthUnit, 2 * heightUnit, chRate, chDepth, chDelay, chFeedback);




}

// auto r = utils::getBoundsWithPadding(this).removeFromTop(comboBoxHeight);
// auto top = r.removeFromTop(comboBoxHeight);
// fxType.setBounds(top.removeFromLeft(160));
// bypass.setBounds(top.removeFromLeft(90));

// wet.setBounds(r.removeFromTop(70));

/*auto row = [&](juce::Component& a, juce::Component& b, juce::Component& c, juce::Component& d)
	{
		auto line = r.removeFromTop(20);
		auto w = utils::getBoundsWithPadding(this).getWidth() / 4;

		a.setBounds(line.removeFromLeft(w).reduced(4));
		b.setBounds(line.removeFromLeft(w).reduced(4));
		c.setBounds(line.removeFromLeft(w).reduced(4));
		d.setBounds(line.removeFromLeft(w).reduced(4));
	};

row(chRate, chDepth, chDelay, chFeedback);
row(flRate, flDepth, flDelay, flFeedback);
row(rvSize, rvDamp, rvWidth, wet);
}*/
