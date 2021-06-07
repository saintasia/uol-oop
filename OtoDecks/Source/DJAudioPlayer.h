/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 25 Jan 2021 11:14:48pm
    Author:  Anastasia Kashkinova

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class DJAudioPlayer : public juce::AudioSource {
    public:
        DJAudioPlayer(juce::AudioFormatManager& _formatManager);
        ~DJAudioPlayer();
        /** implements prepare to play */
        void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
        /** implements getNextAudioBlock */
        void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
        /** implements resouce releasing */
        void releaseResources() override;
        /** loads a track from url */
        void loadURL(juce::URL audioUrl);
        /** sets the volume */
        void setGain(double gain);
        /** sets the playback speed  */
        void setSpeed(double ratio);
        /** sets actual track position */
        void setPosition(double posInSecs);
        /** sets relative track position between 0 and 1 based on the track length */
        void setPositionRelative(double pos);
        
        /** start the player */
        void start();
        /** stop the player */
        void stop();
        /** gets the relative position of the playhead */
        double getPositionRelative();
        /** gets the speed of the track playback */
        double getSpeed();
    
    private:
        juce::AudioFormatManager& formatManager;
        std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
        juce::AudioTransportSource transportSource;
        juce::ResamplingAudioSource resampleSource{&transportSource, false, 2};
};
