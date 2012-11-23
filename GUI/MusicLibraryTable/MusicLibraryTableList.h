//
//  MusicLibraryTableList.h
//  MusicPlayer
//
//  Created by Andy on 23/11/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef MUSICLIBRARYTABLELIST 
#define MUSICLIBRARYTABLELIST

#include "../JuceLibraryCode/JuceHeader.h"
#include "Settings.h"

class MusicLibraryTableList : public TableListBox

{
public:
    MusicLibraryTableList();
    ~MusicLibraryTableList();
    
    void selectedRowsChanged (int lastRowSelected);
    void returnKeyPressed (int currentSelectedRow);
    
    void mouseDoubleClick (const MouseEvent &event); 
    
private:
    
};

#endif
