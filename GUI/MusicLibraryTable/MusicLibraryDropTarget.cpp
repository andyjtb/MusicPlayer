//
//  MusicLibraryDropTarget.cpp
//  MusicPlayer
//
//  Created by Andy on 29/12/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "MusicLibraryDropTarget.h"

MusicLibraryDropTarget::MusicLibraryDropTarget()
{
    //message = "You can also drag-and-drop files here";
    somethingIsBeingDraggedOver = false;
    //setInterceptsMouseClicks(false, false);
    addAndMakeVisible(&musicTable);
}

MusicLibraryDropTarget::~MusicLibraryDropTarget()
{

}

void MusicLibraryDropTarget::resized()
{
    musicTable.setBounds(0,0,getWidth(),getHeight());
}

void MusicLibraryDropTarget::paint(Graphics& g)
{    

}

void MusicLibraryDropTarget::paintOverChildren(Graphics& g)
{
    // draw a red line around the comp if the user's currently dragging something over it..  
    if (somethingIsBeingDraggedOver)
    {
        g.setColour (Colours::red);
        g.drawRect (0, 0, getWidth(), getHeight(), 3);
    }  
}


//FileDragDrop Callbacks
bool MusicLibraryDropTarget::isInterestedInFileDrag (const StringArray &files)
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

void MusicLibraryDropTarget::fileDragEnter (const StringArray &files, int x, int y)
{
    if (isInterestedInFileDrag(files)) {
        somethingIsBeingDraggedOver = true;
        repaint();
    }

}

void MusicLibraryDropTarget::fileDragExit (const StringArray &files)
{
    somethingIsBeingDraggedOver = false;
    repaint();
}
void MusicLibraryDropTarget::filesDropped (const StringArray &files, int x, int y)
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

MusicLibraryTable* MusicLibraryDropTarget::getMusicTable()
{
    return &musicTable;
}