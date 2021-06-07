/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 25 Jan 2021 11:14:48pm
    Author:  Anastasia Kashkinova

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager) : formatManager(_formatManager){};
DJAudioPlayer::~DJAudioPlayer(){};

void DJAudioPlayer::DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate){
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
};

void DJAudioPlayer::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill){
    resampleSource.getNextAudioBlock(bufferToFill);
};

void DJAudioPlayer::releaseResources(){
    transportSource.releaseResources();
    resampleSource.releaseResources();
};

void DJAudioPlayer::loadURL(juce::URL audioURL){
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    
    std::cout << "DJAudioPlayer::loadURL " << audioURL.toString(false) << std::endl;

    if (reader != nullptr) // the file is good
    {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource (new juce::AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    }
};

void DJAudioPlayer::setGain(double gain){
    if(gain < 0 || gain > 1.0) {
        std::cout << "DJAudioPlayer::setGain gain should be between 0 and 1" << std::endl;
    }
    else {
        transportSource.setGain(gain);
    }
};

void DJAudioPlayer::setSpeed(double ratio){
    if(ratio < 0 || ratio > 100.0) {
        std::cout << "DJAudioPlayer::setSpeed ratio should be between 0 and 100" << std::endl;
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }
};

void DJAudioPlayer::setPosition(double posInSecs){
    transportSource.setPosition(posInSecs);
};

void DJAudioPlayer::setPositionRelative(double pos){
    if(pos < 0 || pos > 1.0) {
        std::cout << "DJAudioPlayer::setPositionRelative pos should be between 0 and 1" << std::endl;
    }
    else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
};

void DJAudioPlayer::start(){
    transportSource.start();
};

void DJAudioPlayer::stop(){
    transportSource.stop();
};

double DJAudioPlayer::getPositionRelative(){
double pos = transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
  if(isnan(pos)){
    return 0;
  }
  return pos;
};

double DJAudioPlayer::getSpeed(){
    return resampleSource.getResamplingRatio();
};
