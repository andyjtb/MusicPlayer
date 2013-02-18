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
    
    rootItem = new LibraryViewItem ("Root");//, this);
    rootItem->setOpen (true);
    
    for (int i = 0; i < singletonPlaylistsTree.getNumChildren(); i++) {
        rootItem->addSubItem(new LibraryViewItem(singletonPlaylistsTree.getChild(i).getProperty("Name").toString()));//, this);
    }
    
    treeView.setRootItemVisible(false);
    treeView.setRootItem(rootItem);
}
LibraryTreeView::~LibraryTreeView()
{}

void LibraryTreeView::paint(Graphics &g)
{
    //g.fillAll(Colours::white);
}

void LibraryTreeView::resized()
{
    treeView.setBounds(0,0,getWidth(), getHeight());
}

void LibraryTreeView::setSelected (String incomingPlaylist)
{
    selectedPlaylist = incomingPlaylist;
}

Value LibraryTreeView::getSelectedPlaylistValue()
{
    return selectedPlaylist;
}