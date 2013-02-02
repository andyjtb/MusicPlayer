/*
 *  SearchBox.h
 *  MusicPlayer
 *
 *  Created by Andy on 03/12/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SEARCHBOX
#define SEARCHBOX

#include "../JuceLibraryCode/JuceHeader.h"

class SearchBoxLookAndFeel : public LookAndFeel
{
public:
    SearchBoxLookAndFeel(){}
    //~SearchBoxLookAndFeel(); 
    
    void fillTextEditorBackground (Graphics& g, int width, int height, TextEditor& textEditor)
    {
        if (textEditor.isEnabled())
        {
            g.setColour (Colours::green);
            g.drawRect (0, 0, width, height);
        }
    }
};

class SearchBox  : public Component
{
public:
    //==============================================================================
    SearchBox ();
    ~SearchBox();

    void paint (Graphics& g);
    void resized();

    TextEditor& getSearchTextEditor();
    String getText();
private:

    Label searchLabel;
    TextEditor search;

    SearchBoxLookAndFeel searchBoxLookAndFeel;
};


#endif   //SEARCHBOX
