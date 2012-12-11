/*
 *  SearchBox.cpp
 *  MusicPlayer
 *
 *  Created by Andy on 03/12/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "SearchBox.h"

SearchBox::SearchBox ()
{
    addAndMakeVisible (&searchLabel);
    searchLabel.setText("Search: ", false);

    addAndMakeVisible (&search);
    search.setMultiLine (false);
    search.setReturnKeyStartsNewLine (false);
    search.setReadOnly (false);
    search.setScrollbarsShown (false);
    search.setCaretVisible (true);
    search.setSelectAllWhenFocused(true);
    search.setTextToShowWhenEmpty("Search", Colour (0xff8b8b8b));

    setSize (175, 60);

}

SearchBox::~SearchBox()
{
}

//==============================================================================
void SearchBox::paint (Graphics& g)
{
}

void SearchBox::resized()
{
    searchLabel.setBounds (0, 0, 55, 24);
    search.setBounds (16, 24, 150, 24);
}

TextEditor& SearchBox::getSearchTextEditor()
{
    return search;
}

