//
//  MusicLibraryDropTarget.cpp
//  MusicPlayer
//
//  Created by Andy on 29/12/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "MusicLibraryDropTarget.h"
#include "GuiControl.h"

class AudioControl;


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
    else if (test.getFileExtension().compareIgnoreCase(".mp3") == 0 ||
             test.getFileExtension().compareIgnoreCase(".m4a") == 0 ||
             test.getFileExtension().compareIgnoreCase(".aac") == 0 ||
             test.getFileExtension().compareIgnoreCase(".ogg") == 0 ||
             test.getFileExtension().compareIgnoreCase(".flac") == 0)
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
    for (int counter = 0; counter < files.size(); counter++)
    {
        File current = files[counter];
        if (current.isDirectory())
        {
            DirectoryLoader d;
            d.setWildcards(musicTable.getGuiControl()->getAudioControl()->getRegisteredFormatWildcard());
            d.setDirectory(current);
            
            d.runThread();
        }
        else
        {
            if (current.getFileExtension().compareIgnoreCase(".mp3") == 0 ||
                current.getFileExtension().compareIgnoreCase(".m4a") == 0 ||
                current.getFileExtension().compareIgnoreCase(".aac") == 0 ||
                current.getFileExtension().compareIgnoreCase(".ogg") == 0 ||
                current.getFileExtension().compareIgnoreCase(".flac") == 0)
            {
                singletonLibraryTree.addChild(TagReader::addToLibrary(current),-1,0);
            }
        }
    }
    
    tableUpdateRequired = true;
    //musicTable.updateTableFilteredAndSorted();
    
    somethingIsBeingDraggedOver = false;
    repaint();
}

MusicLibraryTable* MusicLibraryDropTarget::getMusicTable()
{
    return &musicTable;
}