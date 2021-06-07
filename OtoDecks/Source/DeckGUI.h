/*
  ==============================================================================

    DeckGUI.h
    Created: 27 Feb 2021 9:14:38pm
    Author:  Anastasia Kashkinova

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "DBVolControl.h"
#include "CustomLook.h"

//==============================================================================
/*
*/
class DeckGUI : public juce::Component,
                public juce::Button::Listener,
                public juce::Slider::Listener,
                public juce::FileDragAndDropTarget,
                public juce::Timer
{
public:
    DeckGUI(
            DJAudioPlayer* player,
            juce::AudioFormatManager& formatManager,
            juce::AudioThumbnailCache& thumbCache
            );
    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    /** Implement button listener */
    void buttonClicked (juce::Button *) override;
    /** Implement slider listener */
    void sliderValueChanged (juce::Slider *) override;
    /** returns true or false depending on whetehr the file is draggable */
    bool isInterestedInFileDrag (const juce::StringArray &files) override;
    /** implements what happens when files are dropped */
    void filesDropped (const juce::StringArray &files, int x, int y) override;
    /** uses time to repaint */
    void timerCallback() override;
    /** loads a new file to the deck */
    void loadFile(const juce::URL &file);

private:
    /** gets a file name from url */
    std::string getFileName(const juce::URL &file);
    
    // UTF8 icons and icon + word combinations
    juce::String halfIcon = "x"+juce::String::fromUTF8(juce::CharPointer_UTF8("\xc2\xbd"));
    juce::String playIcon = juce::String::fromUTF8(juce::CharPointer_UTF8("\xe2\x80\xa3"))+" Play";
    juce::String rewindIcon = juce::String::fromUTF8(juce::CharPointer_UTF8("\xe2\x86\x90"))+" Re";
    juce::String uploadIcon = juce::String::fromUTF8(juce::CharPointer_UTF8("\xe2\x87\xa7"))+" Load";
    
    // define textButtons
    juce::TextButton playButton{playIcon};
    juce::TextButton stopButton{"|| Pause"};
    juce::TextButton repeatButton{rewindIcon};
    juce::TextButton loadButton{uploadIcon};
    juce::TextButton speedTwiceButton{"x2"};
    juce::TextButton speedNormButton{"x1"};
    juce::TextButton speedHalfButton{halfIcon};
    // define labels
    juce::Label speedSliderLabel{{}, "Speed"};
    juce::Label speedSliderFrom{{}, "0.5"};
    juce::Label speedSliderTo{{}, "2"};
    juce::Label songLabel;
    // define sliders
    juce::Slider speedSlider;
    juce::Slider posSlider;
    // define player reference
    DJAudioPlayer* player;
    // define dbControl and pass it player
    DBVolControl dbControl{player};
    // define waveform
    WaveformDisplay waveformDisplay;
    // define customLook
    CustomLook customLook;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
