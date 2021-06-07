/*
  ==============================================================================

    CustomLook.h
    Created: 13 Mar 2021 12:09:01am
    Author:  Anastasia Kashkinova

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
 Custom Look and Feel component (for DeckGUI)
*/
class CustomLook  : public juce::LookAndFeel_V4
{
public:
    CustomLook();
    ~CustomLook() override;
    
    /* override rotary slider drawing **/
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override;
    /* override linear slider drawing **/
    void drawLinearSlider(juce::Graphics &, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle, juce::Slider &) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomLook)
};
