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
    
    loadPlaylistsTrackInfo();
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
    
    if (message.startsWith("UpdateItems"))
        updateItems();
}

void LibraryTreeView::setSelected(String incomingPlaylist)
{
    selectedPlaylist = incomingPlaylist;
}

String LibraryTreeView::getSelectedPlaylist()
{
    return selectedPlaylist;
}

void LibraryTreeView::loadPlaylistsTrackInfo()
{
    for (int i = 0; i < singletonPlaylistsTree.getNumChildren(); i++)
    {
        ValueTree playlistValueTree = singletonPlaylistsTree.getChild(i);
        
        if (playlistValueTree.isValid()) {
            ValueTree toLoad("TrackInfo");
            
            if (playlistValueTree.getChildWithName("TrackInfo").isValid())
            {
                toLoad = playlistValueTree.getChildWithName("TrackInfo");
                toLoad.removeAllChildren(0);
            }
            
            
            for (int i = 1; i <= int(playlistValueTree.getProperty("Size")); i++)
            {
                int loadID = int(playlistValueTree.getProperty("TrackID" + String(i)));
                ValueTree toAdd = singletonLibraryTree.getChildWithProperty(MusicColumns::columnNames[MusicColumns::ID], loadID).createCopy();
                
                if(toAdd.isValid())
                {
                    toAdd.setProperty("LibID", int(playlistValueTree.getProperty("Size"))+i, 0);
                    toLoad.addChild(toAdd, -1, 0);
                }
            }
            
            playlistValueTree.addChild(toLoad, -1, 0);
        }
    }
}

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