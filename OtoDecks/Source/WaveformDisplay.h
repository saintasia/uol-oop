/*
  ==============================================================================

    WaveformDisplay.h
    Created: 28 Feb 2021 8:42:10pm
    Author:  Anastasia Kashkinova

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformDisplay : public juce::Component,
                        public juce::ChangeListener
{
public:
    WaveformDisplay(juce::AudioFormatManager& formatManager,
                    juce::AudioThumbnailCache& thumbCache);
    ~WaveformDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    /** use  changeListenerCallback to repaint the wave */
    void changeListenerCallback(juce::ChangeBroadcaster *source) override;
    /** load new URL to generate the wave */
    void loadURL(juce::URL audioURL);
    
    /** set relative position of the playhead */
    void setPositionRelative(double pos);

private:
    juce::AudioThumbnail audioThumb;
    bool fileLoaded;
    double position;
    juce::Colour waveColour;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
