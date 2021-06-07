/*
  ==============================================================================

    DeckGUI.cpp
    Created: 27 Feb 2021 9:14:38pm
    Author:  Anastasia Kashkinova

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"
#include "DJAudioPlayer.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
                 juce::AudioFormatManager& formatManager,
                 juce::AudioThumbnailCache& thumbCache
                ) : player(_player),
                waveformDisplay(formatManager, thumbCache)
{
    // set custom look from CustomLook class
    setLookAndFeel(&customLook);
    
    //buttons
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(repeatButton);
    addAndMakeVisible(speedHalfButton);
    addAndMakeVisible(speedNormButton);
    addAndMakeVisible(speedTwiceButton);
    
    // top label
    addAndMakeVisible(songLabel);
    songLabel.setText("", juce::dontSendNotification);
    
    //main graphics
    addAndMakeVisible(waveformDisplay);
    addAndMakeVisible(dbControl);
    addAndMakeVisible(posSlider);
    
    //speedSlider
    addAndMakeVisible(speedSliderLabel);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(speedSliderFrom);
    addAndMakeVisible(speedSliderTo);
    
    // position slider
    posSlider.setSliderStyle(juce::Slider::Rotary);
    posSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    
    // speed slider
    speedSliderLabel.setFont(juce::Font (10.0f, juce::Font::bold));
    speedSliderFrom.setFont(juce::Font (10.0f, juce::Font::bold));
    speedSliderTo.setFont(juce::Font (10.0f, juce::Font::bold));
    speedSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    
    // add event listeners
    playButton.addListener(this);
    stopButton.addListener(this);
    repeatButton.addListener(this);
    loadButton.addListener(this);
    speedHalfButton.addListener(this);
    speedTwiceButton.addListener(this);
    speedNormButton.addListener(this);
    
    posSlider.addListener(this);
    speedSlider.addListener(this);

    // add ranges
    speedSlider.setRange(0.5, 2.0);
    posSlider.setRange(0.0, 1.0);
    
    startTimer(500);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
    setLookAndFeel(nullptr);
}

void DeckGUI::paint (juce::Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);
    g.setColour (juce::Colours::white);
}

void DeckGUI::resized()
{
    double rowH = getHeight()/10;
    double colW = getWidth()/10;
    
    //main graphics
    songLabel.setBounds(0, 0, getWidth(), 20);
    waveformDisplay.setBounds(0, 20, getWidth(), rowH*3 - 20);
    playButton.setBounds(0, rowH*3, colW*2, rowH);
    stopButton.setBounds(colW*2, rowH*3, colW*2, rowH);
    repeatButton.setBounds(colW*4, rowH*3, colW*2, rowH);
    loadButton.setBounds(colW*6, rowH*3, colW*2, rowH);
    posSlider.setBounds(0, rowH*4, colW*8, rowH*5);
    
    //speed functionality
    speedHalfButton.setBounds(0, rowH*9, colW*0.87, rowH);
    speedNormButton.setBounds(colW*0.87, rowH*9, colW*0.87, rowH);
    speedTwiceButton.setBounds(colW*1.77, rowH*9, colW*0.87, rowH);
    speedSliderLabel.setBounds(colW*2.66, rowH*9, colW*5.34, rowH/2);
    speedSliderFrom.setBounds(colW*2.66, rowH*9.5, colW/2, rowH/2);
    speedSlider.setBounds(colW*3.16, rowH*9.5, colW*4.34, rowH/2);
    speedSliderTo.setBounds(colW*7.5, rowH*9.5, colW/2, rowH/2);
    
    //decibel volume control
    dbControl.setBounds(colW*8, rowH*3, colW*2, rowH*7);
}

void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton) {
        player->start();
    }
    if (button == &stopButton) {
        player->stop();
    }
    if (button == &repeatButton) {
        player->setPositionRelative(0.0f);
    }
    if (button == &speedHalfButton) {
        player->setSpeed(0.5f);
    }
    if (button == &speedNormButton) {
        player->setSpeed(1.0f);
    }
    if (button == &speedTwiceButton) {
        player->setSpeed(2.0f);
    }
    if (button == &loadButton) {
        juce::FileChooser chooser{"Select a file..."};
        if (chooser.browseForFileToOpen())
        {
            player->loadURL(juce::URL{chooser.getResult()});
            waveformDisplay.loadURL(juce::URL{chooser.getResult()});
            songLabel.setText(getFileName(juce::URL{chooser.getResult()}), juce::dontSendNotification);
        }
    }
    
}

void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
    if(slider == &speedSlider){
        player->setSpeed(slider->getValue());
    }
    if(slider == &posSlider){
        player->setPositionRelative(slider->getValue());
    }
}

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray &files){
    return true;
};

void DeckGUI::filesDropped(const juce::StringArray &files, int x, int y){
    for (juce::String filename : files)
    {
        juce::URL fileURL = juce::URL{juce::File{filename}};
        player->loadURL(fileURL);
        waveformDisplay.loadURL(fileURL);
        songLabel.setText(getFileName(filename), juce::dontSendNotification);
        return;
    }
};

void DeckGUI::timerCallback(){
    //change position of the waveform head
    waveformDisplay.setPositionRelative(player->getPositionRelative());
    //change the value of the posSlider
    posSlider.setValue(player->getPositionRelative());
    //change the value of the speed slider
    speedSlider.setValue(player->getSpeed());
};

void DeckGUI::loadFile (const juce::URL &file){
    //load into player
    player->loadURL(file);
    //load into wavefore
    waveformDisplay.loadURL(file);
    //set new songLabel
    songLabel.setText(getFileName(file), juce::dontSendNotification);
};


std::string DeckGUI::getFileName(const juce::URL &file){
    std::string trackUrl = file.toString(false).toStdString();
    std::string trackTitle = trackUrl.substr(trackUrl.rfind("/") + 1);
    
    return trackTitle;
};
