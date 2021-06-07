/*
  ==============================================================================

    DBSlider.h
    Created: 12 Mar 2021 1:35:18pm
    Author:  Anastasia Kashkinova

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
 Custom decibel value input/output slider for DBVolControl
 done with the help of the tutorial: https://docs.juce.com/master/tutorial_synth_db_level_control.html
*/
class DBSlider  : public juce::Slider
{
public:
    DBSlider();
    ~DBSlider() override;
    
    /** override Slider's getValueFromText to use decibel input */
    double getValueFromText (const juce::String& text) override;

    juce::String getTextFromValue (double value) override;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DBSlider)
};
