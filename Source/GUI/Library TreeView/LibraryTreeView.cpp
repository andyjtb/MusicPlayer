//
//  LibraryTreeView.cpp
//  MusicPlayer
//
//  Created by Andy on 15/02/2013.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "LibraryTreeView.h"

LibraryTreeView::LibraryTreeView() : treeView("TreeView")
{    
    addAndMakeVisible(&treeView);
    
    rootItem = new LibraryViewItem ("Root");
    rootItem->setOpen (true);
    
    updateItems();
    
    treeView.setRootItemVisible(false);
    treeView.setRootItem(rootItem);
    
    addAndMakeVisible(&plusButton);
    plusButton.addListener(this);
    plusButton.setButtonText("+");
    plusButton.setConnectedEdges(Button::ConnectedOnLeft);
    
    addAndMakeVisible(&minusButton);
    minusButton.addListener(this);
    minusButton.setButtonText("-");
    minusButton.setConnectedEdges(Button::ConnectedOnRight);
}
LibraryTreeView::~LibraryTreeView()
{}

void LibraryTreeView::updateItems()
{
    rootItem->clearSubItems();
    
    for (int i = 0; i < singletonPlaylistsTree.getNumChildren(); i++) {
        LibraryViewItem* currentItem = new LibraryViewItem(singletonPlaylistsTree.getChild(i).getProperty("Name").toString());
        currentItem->addActionListener(this);
        rootItem->addSubItem(currentItem);
    } 
}

void LibraryTreeView::paint(Graphics &g)
{
    //g.fillAll(Colours::white);
}

void LibraryTreeView::resized()
{
    treeView.setBounds(0,0,getWidth(), getHeight()-40);
    
    plusButton.setBounds((getWidth()/2)+22.5, getHeight()-30, 45, 25);
    minusButton.setBounds((getWidth()/2)-22.5, getHeight()-30, 45, 25);
}

void LibraryTreeView::actionListenerCallback (const String& message)
{
    if (message.startsWith("Playlist"))
        {
            String incomingName = message.fromFirstOccurrenceOf("Playlist Changed: ", false, true);
            if (incomingName != selectedPlaylist) {
                selectedPlaylist = incomingName;
                sendChangeMessage();
            }
        }
}

void LibraryTreeView::setSelected(String incomingPlaylist)
{
    selectedPlaylist = incomingPlaylist;
}

String LibraryTreeView::getSelectedPlaylist()
{
    return selectedPlaylist;
}

//ValueTree Callbacks
//void LibraryTreeView::valueTreePropertyChanged (ValueTree &treeWhosePropertyHasChanged, const Identifier &property)
//{}
//void LibraryTreeView::valueTreeChildAdded (ValueTree &parentTree, ValueTree &childWhichHasBeenAdded)
//{
//}
//void LibraryTreeView::valueTreeChildRemoved (ValueTree &parentTree, ValueTree &childWhichHasBeenRemoved)
//{}
//void LibraryTreeView::valueTreeChildOrderChanged (ValueTree &parentTreeWhoseChildrenHaveMoved)
//{}
//void LibraryTreeView::valueTreeParentChanged (ValueTree &treeWhoseParentHasChanged)
//{}

void LibraryTreeView::buttonClicked(Button *button)
{
    if (button == &plusButton) 
    {
        if (singletonPlaylistsTree.isValid())
        {
            AlertWindow addPopup("Add Playlist", "Enter the name for the New Playlist", AlertWindow::NoIcon);
            TextEditor addText;
            
            addText.setBounds(getWidth()/2, getHeight()/2, 300, 25);
            addText.setText("New Playlist");
            
            addPopup.addCustomComponent(&addText);
            
            addPopup.addButton("Cancel", 0);
            addPopup.addButton("Ok", 1);
            
            if (addPopup.runModalLoop() != 0) {
                String playlistName = addText.getText();
                
                bool exists = false;
                
                for (int i = 0; i < singletonPlaylistsTree.getNumChildren(); i++) {
                    if (singletonPlaylistsTree.getChild(i).getProperty("Name") == playlistName) {
                        exists = true;
                        break;
                    }
                }
                
                if (!exists) {
                    singletonCurrentPlaylistId = singletonCurrentPlaylistId+2;
                    ValueTree newPlaylist ("ITEM");
                    newPlaylist.setProperty("Name", addText.getText(), 0);
                    newPlaylist.setProperty("PlaylistID", singletonCurrentPlaylistId, 0);
                    newPlaylist.setProperty("Size", 0, 0);
                    newPlaylist.setProperty("Modified", Time::getCurrentTime().getMilliseconds(), 0);
                    
                    singletonPlaylistsTree.addChild(newPlaylist, -1, 0);
                    
                    LibraryViewItem* currentItem = new LibraryViewItem(playlistName);
                    currentItem->addActionListener(this);
                    rootItem->addSubItem(currentItem);
                }
                else
                {
                    AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Playlist Already Exists", "A playlist with that title already exists\n Please choose another name");
                }
                
            }
            addPopup.removeCustomComponent(0);
        }
    }
    
    if (button == &minusButton)
    {
        if (singletonPlaylistsTree.isValid())
        {
            String title = "Delete " + selectedPlaylist;
            String message = "Are you sure you want to delete " + selectedPlaylist + "?";
            
            AlertWindow removePopup (title, message, AlertWindow::WarningIcon);
            
            removePopup.addButton("Cancel", 0);
            removePopup.addButton("Delete", 1);
            
            if (removePopup.runModalLoop() != 0) 
            {
                //Add undomanager at the end
                ValueTree toDelete = singletonPlaylistsTree.getChildWithProperty("Name", selectedPlaylist);
                if (toDelete.isValid()) 
                {
                    for (int i = 0; i < rootItem->getNumSubItems(); i++) 
                    {
                        if (rootItem->getSubItem(i)->getUniqueName() == toDelete.getProperty("Name").toString()) 
                        {
                            rootItem->removeSubItem(i);
                            break;
                        }
                    }
                    
                    singletonPlaylistsTree.removeChild(toDelete, 0);
                }
                treeView.getItemOnRow(0)->setSelected(true, true);
            }
        }
    }
}