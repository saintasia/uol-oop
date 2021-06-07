/*
  ==============================================================================

    PlaylistComponent.h
    Created: 6 Mar 2021 9:30:09pm
    Author:  Anastasia Kashkinova

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ComboBox.h"
#include "DeckGUI.h"
#include <vector>
#include <string>
#include <tuple>
#include <iostream>
#include <fstream>

//==============================================================================
/*
*/
class PlaylistComponent  :  public juce::Component,
                            public juce::TableListBoxModel,
                            public juce::Button::Listener,
                            public juce::FileDragAndDropTarget,
                            public juce::AudioSource
{
public:
    PlaylistComponent(juce::AudioFormatManager& formatManager, DeckGUI* deckA, DeckGUI* deckB);
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    /* override getNumRows() to be tacks vector length **/
    int getNumRows() override;
    /* override paintRowBackground **/
    void paintRowBackground (
         juce::Graphics&,
         int rowNumber,
         int width,
         int height,
         bool rowIsSelected
    ) override;
    
    /* override paintCell to use custom components **/
    void paintCell (
        juce::Graphics&,
        int rowNumber,
        int columnId,
        int width,
        int height,
        bool rowIsSelected
    ) override;
    
    /* override selectedRowsChanged to be able to send the data to combobox **/
    void selectedRowsChanged(int lastRowSelected) override;
    
    /* override refreshComponentForCell to have the ability to track components inside cells **/
    juce::Component* refreshComponentForCell (
      int rowNumber,
      int columnId,
      bool isRowSelected,
      juce::Component *existingComponentToUpdate
    ) override;
    
    
    // AudioSource overrides
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    
    // Interactivity listeners (Button + FileDragAndDropTarget)
    void buttonClicked(juce::Button* button) override;
    bool isInterestedInFileDrag (const juce::StringArray &files) override;
    void filesDropped (const juce::StringArray &files, int x, int y) override;

private:
    // declare tableComponent
    juce::TableListBox tableComponent;
    
    // declare transportSource and formatManager
    juce::AudioTransportSource transportSource;
    juce::AudioFormatManager& formatManager;
    
    // private function for adding files on drag and drop
    void addFile(juce::URL audioUrl);
    
    // readersource vector
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    // tracks vector of tuples
    std::vector<std::tuple<std::string, std::string, juce::URL, int>> tracks;
    
    // initialize ComboBox and pass it tableComponent
    ComboBox comboBox{tableComponent};
    // define passed in DeckGUI vars
    DeckGUI* deckA;
    DeckGUI* deckB;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
