//
//  LibraryTreeView.cpp
//  MusicPlayer
//
//  Created by Andy on 02/02/2013.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "LibraryTreeView.h"

LibraryTreeView::LibraryTreeView()
{
    treeView.setDefaultOpenness(true);
    treeView.setMultiSelectEnabled(false);
    treeView.setOpenCloseButtonsVisible(true);
    treeView.setRootItemVisible(true);
    
    libraryXml = new XmlElement("Library");
    
    libraryXml->addTextElement("Music");
    
    XmlElement* playlistsXml = new XmlElement("Playlists");
    libraryXml->addChildElement(playlistsXml);
    
    playlistsValueTree = singletonPlaylistsTree;
    if (playlistsValueTree.isValid()) {
        for (int i = 0; i < playlistsValueTree.getNumChildren(); i++)
        {
            playlistsXml->addTextElement(playlistsValueTree.getChild(i).getProperty("PlaylistName"));
        }
    }

    
    rootItem = new LibraryTreeViewItem(*libraryXml);
    treeView.setRootItem(rootItem);
    
}

LibraryTreeView::~LibraryTreeView()
{}


void LibraryTreeView::resized()
{
    treeView.setBounds(0, 0, getWidth(), getHeight());
}


