//
//  FileDropTarget.h
//  MusicPlayer
//
//  Created by Andy on 29/12/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef FILEDROPTARGET 
#define FILEDROPTARGET 

#include "../JuceLibraryCode/JuceHeader.h"
#include "DirectoryLoader.h"

class FileDropTarget  :  public Component,
                            public FileDragAndDropTarget
{
public:
    FileDropTarget ();
    ~FileDropTarget();
    
    void paint (Graphics& g);
    
    //FileDragDrop Callbacks
    bool isInterestedInFileDrag (const StringArray &files);
    void fileDragEnter (const StringArray &files, int x, int y);
    void fileDragExit (const StringArray &files);
    void filesDropped (const StringArray &files, int x, int y);
	
private:
    String message;
    bool somethingIsBeingDraggedOver;
    
};

#endif
