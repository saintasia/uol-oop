/*
  ==============================================================================

    DBVolControl.h
    Created: 11 Mar 2021 11:00:53pm
    Author:  Anastasia Kashkinova

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DBSlider.h"
#include "DJAudioPlayer.h"

//==============================================================================
/*
*/
class DBVolControl  : public juce::Component
{
public:
    DBVolControl(DJAudioPlayer* player);
    ~DBVolControl() override;
    
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DBVolControl)
    // custom decibel slider
    DBSlider dbControl;
    // slider label
    juce::Label dbControlLabel;
    // volume level
    float level = 2.0f;
    // player reference
    DJAudioPlayer* player;
    // volume fill colour
    juce::Colour volColour;
};
