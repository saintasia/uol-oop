/*
  ==============================================================================

    ComboBox.h
    Created: 8 Mar 2021 5:21:48pm
    Author:  Anastasia Kashkinova

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ComboBox  : public juce::Component
{
public:
    ComboBox(juce::TableListBox& tableComponent);
    ~ComboBox() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    /** add new item to combobox list */
    void addItem(std::string item, int id);
    /** listens to dropdown menu being changed */
    void menuChanged();
    /** listens to the typed text being changed */
    void textChanged();
    /** resets the text in the field to nothing */
    void resetText(std::string text);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ComboBox)
    // guiding text
    juce::Label textLabel { {}, "Search: Type first letters and press enter to autofill, or use dropdown"};
    juce::Font textFont { 12.0f };
    // combobox to use under the hood
    juce::ComboBox comboBox;
    // reference to the tableComponent
    juce::TableListBox& tableComponent;
    // vector of tracks(name strings and ids)
    std::vector<std::tuple<std::string, int>> tracks;
};
