//
//  MusicLibraryTableList.cpp
//  MusicPlayer
//
//  Created by Andy on 23/11/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "MusicLibraryTableList.h"

MusicLibraryTableList::MusicLibraryTableList()
{}

MusicLibraryTableList::~MusicLibraryTableList()
{}

void MusicLibraryTableList::selectedRowsChanged(int lastRowSelected)
{
    tableSelectedRow.setValue(lastRowSelected);
}

void MusicLibraryTableList::returnKeyPressed(int currentSelectedRow)
{
    tableShouldPlay.setValue(true);
    tableSelectedRow.setValue(currentSelectedRow);    
}

void MusicLibraryTableList::mouseDoubleClick(const MouseEvent &event)
{
    DBG("Clicked");
    //tableShouldPlay.setValue(true);
    //tableSelectedRow.setValue(getSelectedRow());   
}