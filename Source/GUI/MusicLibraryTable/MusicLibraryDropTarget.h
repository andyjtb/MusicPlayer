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
/** Allows files and directories to be dropped onto the MusicLibraryTable and get added to the library */
class MusicLibraryDropTarget  :  public Component,
                                 public FileDragAndDropTarget,
                                 public DragAndDropContainer
{
public:
    
    MusicLibraryDropTarget ();
    ~MusicLibraryDropTarget();
    
    /** @internal */
    void resized();
    /** @internal */
    void paint (Graphics& g);
    /** Draws red outline around table if file/directory is currently over the table. Has to be over children to draw on top of the MusicLibraryTable */
    void paintOverChildren(Graphics& g);
    
    //FileDragDrop Callbacks
    /** Whether the class is interested in files being dragged into it */
    bool isInterestedInFileDrag (const StringArray &files);
    /** Calls for a red outline to be drawn around the table when a file/directory is dragged over the table */
    void fileDragEnter (const StringArray &files, int x, int y);
    /** Called when there is no longer anything hovering over the table - Calls for the red outline to be removed */
    void fileDragExit (const StringArray &files);
    /** Loads all supported files in the directory which has been dropped on the target */
    void filesDropped (const StringArray &files, int x, int y);
	/** Returns a pointer to the MusicLibraryTable that this class contains */
    MusicLibraryTable* getMusicTable();
    
private:
    String message;
    bool somethingIsBeingDraggedOver;
    
    MusicLibraryTable musicTable;
    
};

#endif
