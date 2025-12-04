/*
  ==============================================================================

    LimiterComponent.cpp
    Created: 4 Dec 2025 3:18:25pm
    Author:  crist

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LimiterComponent.h"
#include "Utils.h"

LimiterComponent::LimiterComponent(juce::AudioProcessorValueTreeState& apvts,
                                   juce::String thresholdId,
                                   juce::String releaseMsId)
    : threshLS("Threshold", apvts, thresholdId, *this, juce::Slider::LinearBarVertical, true)
    , releaseLS("Release", apvts, releaseMsId, *this, juce::Slider::LinearBarVertical, true)
{
    threshLS.setUnitSuffix(" dB");
    releaseLS.setUnitSuffix(" ms");
}

LimiterComponent::~LimiterComponent() = default;

void LimiterComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    utils::drawBorders(g, this, utils::miscCol, "Limiter");
}

void LimiterComponent::resized()
{
    auto content = utils::getContentArea(this);

    utils::layoutVisibleRow(content.getX(), content.getY(),
                            content.getWidth(), content.getHeight(),
                            { &threshLS, &releaseLS });
}
