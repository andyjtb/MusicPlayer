//
//  LibraryTreeView.h
//  MusicPlayer
//
//  Created by Andy on 15/02/2013.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef MusicPlayer_LibraryTreeView_h
#define MusicPlayer_LibraryTreeView_h

#include "../JuceLibraryCode/JuceHeader.h"
#include "Settings.h"
#include "LibraryTreeViewBinaryData.h"
#include "Utility.h"

//==============================================================================
class LibraryViewItem  : public TreeViewItem,
                         public ActionBroadcaster
{
public:
    LibraryViewItem (String incomingPlaylistTitle)
    {
        //parent = incomingParent;
        
        libraryIcon = ImageCache::getFromMemory (LibraryTreeViewBinaryData::small_library_png, LibraryTreeViewBinaryData::small_library_pngSize);
        playlistIcon = ImageCache::getFromMemory (LibraryTreeViewBinaryData::small_playlist_png, LibraryTreeViewBinaryData::small_playlist_pngSize);

        playlistTitle = incomingPlaylistTitle;
        
        setLinesDrawnForSubItems(false);  
    }
    
    void itemSelectionChanged(bool isNowSelected)
    {
        if (isNowSelected)
        {
            sendActionMessage("Playlist Changed: " + playlistTitle);
        }
    }
    
    int getItemWidth() const
    {
        return 150;
    }
    
    int getItemHeight() const
    {
        return 30;
    }
    
    String getUniqueName() const
    {
        return String(playlistTitle);
    }

    bool mightContainSubItems()
    {
        return 0;//xml->getFirstChildElement() != 0;
    }
    
    void paintItem (Graphics& g, int width, int height)
    {
        g.setFont (Font (17.0f, Font::plain));
        
        if (playlistTitle == "Library")
        {
            g.drawImage (libraryIcon,
                         0, 5, 20, 22,
                         0, 0, libraryIcon.getWidth(), libraryIcon.getHeight());
            
            g.drawText (playlistTitle,
                        22, 0, width - 4, height,
                        33, true);
        }
        else
        {
            g.drawImage (playlistIcon,
                         0, 3, 20, 22,
                         0, 0, playlistIcon.getWidth(), playlistIcon.getHeight());
            
            g.drawText (playlistTitle,
                        22, 0, width - 4, height,
                        33, true);
        }
        
        // if this item is selected, fill it with a background colour..
        if (isSelected())
        {
            //parent->setSelectedPlaylist(playlistTitle);
            g.fillAll (Colours::blue.withAlpha (0.2f));
        }
        
    }
    
    void itemOpennessChanged (bool isNowOpen)
    {
    }
    
    bool isInterestedInDragSource (const DragAndDropTarget::SourceDetails& dragSourceDetails)
    {
        if (playlistTitle != "Library") 
        {
            if (dragSourceDetails.description.getArray() != nullptr) 
            {
                return true;
            }
        }
        return false;
    }
    
    void itemDropped (const DragAndDropTarget::SourceDetails& dragSourceDetails, int insertIndex)
    {
        Array<var>* incomingArray = dragSourceDetails.description.getArray();
        
        if (incomingArray != nullptr) {
            ValueTree currentPlaylist = singletonPlaylistsTree.getChildWithProperty("Name", playlistTitle);
            
            if(currentPlaylist.isValid())
            {
                int size = currentPlaylist.getProperty("Size");
                                
                for (int i = 0; i < incomingArray->size(); i++)
                {
                    String trackId("TrackID");
                    size++;
                    trackId << size;

                    var libID = incomingArray->getReference(i);
                    DBG(trackId << " " <<libID.toString());
                    currentPlaylist.setProperty(trackId, libID, 0);
                }
                
                currentPlaylist.setProperty("Size", size, 0);
            }
        }
    }
    
private:
    String playlistTitle;
    
    Image playlistIcon;
    Image libraryIcon;
    
    //LibraryTreeView* parent;
};

class LibraryTreeView : public Component,
                        public ActionListener,
                        public ChangeBroadcaster,
                        //public ValueTree::Listener,
                        public Button::Listener
{
public:
    LibraryTreeView();
    ~LibraryTreeView();
    
    void updateItems();
    
    void paint(Graphics &g);
    void resized();
    
    void setSelected (String incomingPlaylist);
    String getSelectedPlaylist();
    
    void actionListenerCallback (const String& message);
    
    void buttonClicked (Button* button);
    
//    void valueTreePropertyChanged (ValueTree &treeWhosePropertyHasChanged, const Identifier &property);
//    void valueTreeChildAdded (ValueTree &parentTree, ValueTree &childWhichHasBeenAdded);
//    void valueTreeChildRemoved (ValueTree &parentTree, ValueTree &childWhichHasBeenRemoved);
//    void valueTreeChildOrderChanged (ValueTree &parentTreeWhoseChildrenHaveMoved);
//    void valueTreeParentChanged (ValueTree &treeWhoseParentHasChanged);
    
private:
    TreeView treeView;
    ScopedPointer<LibraryViewItem> rootItem;
    
    String selectedPlaylist;
    
    TextButton plusButton, minusButton;
};

#endif
