/*
  ==============================================================================

    DBVolControl.cpp
    Created: 11 Mar 2021 11:00:53pm
    Author:  Anastasia Kashkinova

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DBVolControl.h"

//==============================================================================
DBVolControl::DBVolControl(DJAudioPlayer* _player) : player(_player)
{
    
    // custom colours vector
    std::vector<juce::Colour> MyColors;
    MyColors.push_back(juce::Colours::mediumorchid);
    MyColors.push_back(juce::Colours::turquoise);
    MyColors.push_back(juce::Colours::mediumseagreen);
    MyColors.push_back(juce::Colours::orangered);
    
    // get a random color for volune filling
    std::random_shuffle(MyColors.begin(), MyColors.end());
    volColour = MyColors[0];
    
    // set range
    dbControl.setRange(-100, 0);
    dbControlLabel.setText("Vol dB", juce::dontSendNotification);
    // set value change
    dbControl.onValueChange = [this] {
        // chane DB to gain value
        level = juce::Decibels::decibelsToGain((float) dbControl.getValue());
        // and set it via player
        player->setGain(level);
        repaint();
    };
    // set the new db Value
    dbControl.setValue(juce::Decibels::gainToDecibels(level));
    
    // slider styles (hide the native styles)
    dbControl.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    dbControl.setColour(DBSlider::backgroundColourId, juce::Colours::transparentBlack);
    dbControl.setColour(DBSlider::trackColourId, juce::Colours::transparentBlack);
    dbControl.setColour(DBSlider::thumbColourId, juce::Colours::transparentBlack);
    dbControl.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    
    addAndMakeVisible(dbControl);
    addAndMakeVisible(dbControlLabel);
}

DBVolControl::~DBVolControl()
{
}

void DBVolControl::paint (juce::Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);
    
    // draw custom slider instead
    juce::ColourGradient gradientFill = juce::ColourGradient(volColour, 60, 200,juce::Colours::darkgrey, 0.0, 0.0, true);
    g.setGradientFill(gradientFill);
    g.setFont(14.0f); // draw
    double volHeight = getHeight()/100 * (dbControl.getValue() + 100);
    g.fillRect(0, getHeight()-volHeight, getWidth(), volHeight);
    
}

void DBVolControl::resized()
{
    dbControlLabel.setBounds (0, 10, getWidth(), 20);
    dbControl.setBounds(0, 0, getWidth(), getHeight());
}
