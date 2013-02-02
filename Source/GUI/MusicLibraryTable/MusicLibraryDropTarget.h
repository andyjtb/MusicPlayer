//
//  MusicLibraryDropTarget.h
//  MusicPlayer
//
//  Created by Andy on 29/12/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef MUSICLIBRARYDROPTARGET 
#define MUSICLIBRARYDROPTARGET 

#include "../JuceLibraryCode/JuceHeader.h"
#include "DirectoryLoader.h"
#include "MusicLibraryTableModel.h"

class MusicLibraryDropTarget  :  public Component,
                            public FileDragAndDropTarget
{
public:
    MusicLibraryDropTarget ();
    ~MusicLibraryDropTarget();
    
    void resized();
    void paint (Graphics& g);
    void paintOverChildren(Graphics& g);
    
    //FileDragDrop Callbacks
    bool isInterestedInFileDrag (const StringArray &files);
    void fileDragEnter (const StringArray &files, int x, int y);
    void fileDragExit (const StringArray &files);
    void filesDropped (const StringArray &files, int x, int y);
	
    MusicLibraryTable* getMusicTable();
    
private:
    String message;
    bool somethingIsBeingDraggedOver;
    
    MusicLibraryTable musicTable;
    
};

#endif
