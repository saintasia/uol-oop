/*
  ==============================================================================

    ComboBox.cpp
    Created: 8 Mar 2021 5:21:48pm
    Author:  Anastasia Kashkinova

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ComboBox.h"

//==============================================================================
ComboBox::ComboBox(juce::TableListBox& _tableComponent) : tableComponent(_tableComponent)
{
    // add all items
    addAndMakeVisible(textLabel);
    textLabel.setFont(textFont);
    addAndMakeVisible(comboBox);
    
    // set selected id to 0 initially
    comboBox.setSelectedId(0);
    comboBox.setEditableText(true);
    // on comboBox change, use custom menuChanged() function
    comboBox.onChange = [this] { menuChanged(); };
}

ComboBox::~ComboBox()
{
}

void ComboBox::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);
    g.setColour (juce::Colours::white);
}

void ComboBox::resized()
{
    textLabel.setBounds(0, 0, getWidth()/2, getHeight());
    comboBox.setBounds(getWidth()/2, 0, getWidth()/2, getHeight());
}

void ComboBox::addItem(std::string item, int id){
    // add an item to combobox
    comboBox.addItem(item, id);
    // add push tuple with the same information to tracks
    tracks.push_back(std::make_tuple(item, id));
}

void ComboBox::menuChanged(){
    int selectedId = comboBox.getSelectedId();
    // trigger custom textChanged function
    textChanged();
    // set the new row to be selected
    tableComponent.selectRow(selectedId-1);
};

void ComboBox::textChanged(){
    bool changeIsFound = false;
    // get the current text
    std::string text = comboBox.getText().toStdString();
    // traverse through tracks vector
    for (int i=0; i < tracks.size(); ++i){
        std::string title = std::get<0>(tracks[i]);
        
        // if the full text value is in our tracks list, do nothing
        if (text == title) return;
        
        // otherwise look for a first match
        int id = std::get<1>(tracks[i]);
        // if a match has been found
        std::size_t found = title.find(text);
        if (found!=std::string::npos) {
            // set it to be current active item
            comboBox.setText(title);
            comboBox.setSelectedId(id);
            changeIsFound = true;
            return;
        }
    }
    
    // if no match is found
    if (!changeIsFound){
        comboBox.setText("No matches found");
    }
};

void ComboBox::resetText(std::string text){
    comboBox.setText(text);
}
