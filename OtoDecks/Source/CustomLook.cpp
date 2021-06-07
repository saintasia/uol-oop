/*
  ==============================================================================

    CustomLook.cpp
    Created: 13 Mar 2021 12:09:01am
    Author:  Anastasia Kashkinova

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CustomLook.h"

//==============================================================================
CustomLook::CustomLook()
{
}

CustomLook::~CustomLook()
{
}

void CustomLook::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&)
{
    auto radius = (float) juce::jmin (width / 2, height / 2) - 4.0f;
    auto radiusSm = (float) juce::jmin (width / 5, height / 5) - 4.0f;
    auto midX = (float) x + (float) width  * 0.5f;
    auto midY = (float) y + (float) height * 0.5f;
    auto radx = midX - radius;
    auto rady = midY - radius;
    auto radw = radius * 2.0f;
    auto radxSm = midX - radiusSm;
    auto radySm = midY - radiusSm;
    auto radwSm = radiusSm * 2.0f;
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // disk style
    g.setColour(juce::Colours::black);
    g.setOpacity(0.7);
    g.fillEllipse(radx, rady, radw, radw);
    g.setColour(juce::Colours::lightslategrey);
    g.drawEllipse(radx, rady, radw, radw, 1.0f);
    g.setColour(juce::Colours::lightslategrey);
    g.fillEllipse(radxSm, radySm, radwSm, radwSm);

    // pointer styles
    juce::Path path;
    auto pointerLength = radius * 0.66f;
    auto pointerThickness = 2.0f;
    path.addRectangle(-pointerThickness * 1, -radius, pointerThickness, pointerLength);
    path.applyTransform(juce::AffineTransform::rotation (angle).translated (midX, midY));

    g.setColour (juce::Colours::orchid);
    g.fillPath(path);
}

void CustomLook::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle style, juce::Slider &){
    
    if(style == juce::Slider::SliderStyle::LinearHorizontal){
        g.setColour(juce::Colours::black);
        g.setOpacity(0.7);
        g.fillRect(x, y, width, height);
        
        // pointer styles
        juce::Path path;
        path.addRectangle(sliderPos, 0, 5.0f, height);
        g.setColour(juce::Colours::orchid);
        g.fillPath(path);
    };
}
