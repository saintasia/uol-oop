/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 28 Feb 2021 8:42:10pm
    Author:  Anastasia Kashkinova

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManager,
                                 juce::AudioThumbnailCache& thumbCache) :
                                    audioThumb(1000, formatManager, thumbCache),
                                    fileLoaded(false),
                                    position(0)
{
    // colours array
    std::vector<juce::Colour> MyColors;
    MyColors.push_back(juce::Colours::mediumorchid);
    MyColors.push_back(juce::Colours::turquoise);
    MyColors.push_back(juce::Colours::mediumseagreen);
    MyColors.push_back(juce::Colours::orangered);
    
    std::random_shuffle(MyColors.begin(), MyColors.end());
    // set waveColour to a random value from the colours array
    waveColour = MyColors[0];
    // listen to changes and pass referene
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (juce::Graphics& g)
{
 
    g.fillAll (getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.drawRect (getLocalBounds(), 1);

    g.setColour (juce::Colours::aquamarine); // in case gradient doesn't work
    // fill the component with a gradient based on the semi-random colour we got above
    juce::ColourGradient gradientFill = juce::ColourGradient(waveColour, 0, getHeight()/2, juce::Colours::lightgoldenrodyellow, getWidth(), 0.0, false);
    g.setGradientFill(gradientFill);
    
    if(fileLoaded){
        audioThumb.drawChannel(
           g,
           getLocalBounds(),
           0,
           audioThumb.getTotalLength(),
           0,
           1.0f
        );
        // draw the pointer
        g.setColour(juce::Colours::orchid);
        g.drawRect(position * getWidth(), 0, 2, getHeight());
    }else{
        // default text
        g.setFont (20.0f);
        g.drawText ("File not loaded...", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
    }
}

void WaveformDisplay::resized()
{
}

void WaveformDisplay::loadURL(juce::URL audioURL){
    // clear previous thumb
    audioThumb.clear();
    
    // repaint
    fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));
    if(fileLoaded){
        repaint();
    }
}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster *source)
{
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos){
    if(pos != position){
        position = pos;
        repaint();
    }
};

