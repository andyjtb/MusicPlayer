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

class SearchBox  : public Component
{
public:
    //==============================================================================
    SearchBox ();
    ~SearchBox();

    void paint (Graphics& g);
    void resized();

    TextEditor& getSearchTextEditor();
private:

    Label searchLabel;
    TextEditor search;

};


#endif   //SEARCHBOX
