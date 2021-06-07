/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 6 Mar 2021 9:30:09pm
    Author:  Anastasia Kashkinova

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include "DeckGUI.h"
#include <cmath>

//==============================================================================
PlaylistComponent::PlaylistComponent(juce::AudioFormatManager& _formatManager, DeckGUI* _deckA, DeckGUI* _deckB) : formatManager(_formatManager), deckA(_deckA), deckB(_deckB)
{
    
    // set table basics
    double unit = 100;
    tableComponent.getHeader().addColumn("Track title", 1, unit * 4);
    tableComponent.getHeader().addColumn("Length", 2, unit * 2);
    tableComponent.getHeader().addColumn("A", 3, unit);
    tableComponent.getHeader().addColumn("B", 4, unit);
    tableComponent.setModel(this);
    
    tableComponent.setHeaderHeight(30);
    
    addAndMakeVisible(tableComponent);
    addAndMakeVisible(comboBox);
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (juce::Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);
    g.setColour (juce::Colours::white);
}

void PlaylistComponent::resized()
{
    comboBox.setBounds(0, 0, getWidth(), 50);
    tableComponent.setBounds(0, 50, getWidth(), getHeight() - 50);
}

int PlaylistComponent::getNumRows(){
    return (int) tracks.size();
};

void PlaylistComponent::paintRowBackground (
    juce::Graphics& g,
    int rowNumber,
    int width,
    int height,
    bool rowIsSelected){
        //set custom look
        if (rowIsSelected){
            g.fillAll(juce::Colours::darkorchid);
            g.setColour(juce::Colours::white);
        }else{
            g.setOpacity(0.6);
            g.fillAll(juce::Colours::black);
            g.setColour(juce::Colours::lightslategrey);
        }
};

void PlaylistComponent::paintCell (
    juce::Graphics& g,
    int rowNumber,
    int columnId,
    int width,
    int height,
    bool rowIsSelected){
        if (columnId == 1){
            //output the track name in the first col
            g.drawText(
               std::get<0>(tracks[rowNumber]),
               2,
               0,
               width - 4,
               height,
               juce::Justification::centredLeft
            );
        }
        if (columnId == 2){
            //output the track length in the second col
            g.drawText(
               std::get<1>(tracks[rowNumber]),
               3,
               0,
               width - 4,
               height,
               juce::Justification::centredLeft
            );
            
        }
};

juce::Component* PlaylistComponent::refreshComponentForCell (
    int rowNumber,
    int columnId,
    bool isRowSelected,
    juce::Component *existingComponentToUpdate){
        // if the column id is 3 or 4, override the contents
        if(columnId == 3){
            //output the load A button in col 3
            if(existingComponentToUpdate == nullptr){
                juce::TextButton* btn = new juce::TextButton{"Load A"};
                juce::String id{std::to_string(rowNumber) + "_A"};
                btn->setComponentID(id);
                btn->addListener(this);
                existingComponentToUpdate = btn;
            }
        }
        if(columnId == 4){
            //output the load B button in col 4
            if(existingComponentToUpdate == nullptr){
                juce::TextButton* btn = new juce::TextButton{"Load B"};
                juce::String id{std::to_string(rowNumber) + "_B"};
                btn->setComponentID(id);
                btn->addListener(this);
                existingComponentToUpdate = btn;
            }
        }
        return existingComponentToUpdate;
};


void PlaylistComponent::selectedRowsChanged(int lastRowSelected){
    std::string selectedTitle = std::get<0>(tracks[lastRowSelected]);
    // TODO: find a way to reset comboBox text on new row selection
    // comboBox.resetText(selectedTitle); // this doesn't work because it triggers text change and the component events are interdependent
};

void PlaylistComponent::buttonClicked(juce::Button* button){
    // get componentID
    std::string receivedId = button->getComponentID().toStdString();
    // split string to get the deck code
    std::string deck = receivedId.substr(receivedId.rfind("_") + 1);
    // split string to get the id of the row
    int id = std::stoi(receivedId.substr(0, receivedId.find("_")));
    // grab fileURL from tracks vector and onClick load to respective decks
    juce::URL fileURL = std::get<2>(tracks[id]);
    if (deck == "A") {
        deckA->loadFile(fileURL);
    }
    if (deck == "B") {
        deckB->loadFile(fileURL);
    }
};

bool PlaylistComponent::isInterestedInFileDrag (const juce::StringArray &files){
    return true;
};

void PlaylistComponent::filesDropped (const juce::StringArray &files, int x, int y){
    // add all the dropped files
    for (int i = 0; i < files.size(); ++i)
    {
        addFile(juce::URL{juce::File{files[i]}});
    }
};

void PlaylistComponent::addFile(juce::URL audioURL){
    // initialise the reader
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));

    if (reader != nullptr) // if the file is good
    {
        // set new sources
        std::unique_ptr<juce::AudioFormatReaderSource> newSource (new juce::AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset (newSource.release());
        
        // create meta
        juce::URL originalUrl = audioURL;
        std::string trackUrl = audioURL.toString(false).toStdString();
        std::string trackTitle = trackUrl.substr(trackUrl.rfind("/") + 1);
        int id = (int) tracks.size() + 1;
        int mins = (int) floor(transportSource.getLengthInSeconds() / 60);
        int secs = (int) (transportSource.getLengthInSeconds() - mins * 60);
        std::string trackLength = std::to_string(mins) + "min" + std::to_string(secs) + "secs";
        
        // push meta to tracks
        tracks.push_back(std::make_tuple(trackTitle, trackLength, originalUrl, id));
        
        // update the table
        tableComponent.updateContent();
        
        // update the combobox search
        comboBox.addItem(trackTitle, id);
        repaint();
    }
};

void PlaylistComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate){
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
};

void PlaylistComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill){
};

void PlaylistComponent::releaseResources(){
    transportSource.releaseResources();
};
