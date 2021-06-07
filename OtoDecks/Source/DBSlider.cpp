/*
  ==============================================================================

    DBSlider.cpp
    Created: 12 Mar 2021 1:35:18pm
    Author:  Anastasia Kashkinova

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DBSlider.h"

//==============================================================================
DBSlider::DBSlider()
{
}

DBSlider::~DBSlider()
{
}

double DBSlider::getValueFromText(const juce::String& text){
    auto minusInfinitydB = -100.0;

    auto decibelText = text.upToFirstOccurrenceOf ("dB", false, false).trim();

    return decibelText.equalsIgnoreCase ("-INF") ? minusInfinitydB
                                                 : decibelText.getDoubleValue();
}

juce::String DBSlider::getTextFromValue(double value){
    return juce::Decibels::toString (value);
}

