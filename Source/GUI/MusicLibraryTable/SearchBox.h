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

#include "JuceHeader.h"

/** The search box */
class SearchBox  : public Component
{
public:
    //==============================================================================
    SearchBox ();
    ~SearchBox();

    /** @internal */
    void paint (Graphics& g) override;
    /** @internal */
    void resized() override;

    /** Returns a reference to the TextEditor so listeners can be added */
    TextEditor& getSearchTextEditor();
    /** Returns the text currently being displayed in the search box */
    String getText();
private:

    Label searchLabel;
    TextEditor search;
};


#endif   //SEARCHBOX
