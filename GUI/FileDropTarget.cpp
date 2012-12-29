//
//  FileDropTarget.cpp
//  MusicPlayer
//
//  Created by Andy on 29/12/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "FileDropTarget.h"

FileDropTarget::FileDropTarget()
{
    message = "You can also drag-and-drop files here";
    somethingIsBeingDraggedOver = false;
    //setInterceptsMouseClicks(false, false);
}

FileDropTarget::~FileDropTarget()
{

}

void FileDropTarget::paint(Graphics& g)
{  
    g.fillAll (Colours::green.withAlpha (0.2f));
    
    // draw a red line around the comp if the user's currently dragging something over it..
    if (somethingIsBeingDraggedOver)
    {
        g.setColour (Colours::red);
        g.drawRect (0, 0, getWidth(), getHeight(), 3);
    }
    
    g.setColour (Colours::black);
    g.setFont (14.0f);
    g.drawFittedText (message, getLocalBounds().reduced (10, 0), Justification::centred, 4);
    
}


//FileDragDrop Callbacks
bool FileDropTarget::isInterestedInFileDrag (const StringArray &files)
{
    File test(files[0]);
    if (test.isDirectory())
    {
        return true;
    }
    else if (files[0].endsWith(".mp3"))
    {
        return true;
    }
    return false;
}

void FileDropTarget::fileDragEnter (const StringArray &files, int x, int y)
{
    somethingIsBeingDraggedOver = true;
    repaint();

    if (isInterestedInFileDrag(files)) {
        DBG("Is Interested");
    }

}

void FileDropTarget::fileDragExit (const StringArray &files)
{
    somethingIsBeingDraggedOver = false;
    repaint();
}
void FileDropTarget::filesDropped (const StringArray &files, int x, int y)
{
    //The good shit
    for (int counter = 0; counter < files.size(); counter++)
    {
        File current = files[counter];
        if (current.isDirectory())
        {
            DirectoryLoader d;
            d.setDirectory(current);
            
            if (d.runThread())
            {
                DBG("Complete");
            }
            else
            {
                DBG("User quit early");
            }
        }
        else
        {
            if (current.getFileName().endsWith(".mp3")) {
                singletonLibraryTree.addChild(TagReader::addToLibrary(current),-1,0);
            }
        }
    }
    
    somethingIsBeingDraggedOver = false;
    repaint();
}