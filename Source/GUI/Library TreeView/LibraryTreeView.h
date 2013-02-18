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

//==============================================================================
class LibraryViewItem  : public TreeViewItem
{
public:
    LibraryViewItem (String incomingPlaylistTitle)
    {
        //parent = incomingParent;
        
        libraryIcon = ImageCache::getFromMemory (LibraryTreeViewBinaryData::small_library_png, LibraryTreeViewBinaryData::small_library_pngSize);
        playlistIcon = ImageCache::getFromMemory (LibraryTreeViewBinaryData::small_playlist_png, LibraryTreeViewBinaryData::small_playlist_pngSize);
        
        playlistTitle = incomingPlaylistTitle;
        
        setLinesDrawnForSubItems(false);
        //isLibrary = isTypeLibrary;
        //label = createItemComponent();
//        if(label)
//        {
//            label->setText("helo");
//            
//            DBG("Label text - " << label->getText());
//        }
//        else
//            DBG("Label not made");
        
    }
    
//    void paint()
//    {
//        label->setSelected(isSelected());
//        treeHasChanged();
//    }
    
    void itemSelectionChanged(bool isNowSelected)
    {
        if (isNowSelected)
        DBG(playlistTitle << " clicked");
    }
    
    int getItemWidth() const
    {
        return 100;
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
        if (isNowOpen)
        {
            // if we've not already done so, we'll now add the tree's sub-items. You could
            // also choose to delete the existing ones and refresh them if that's more suitable
            // in your app.
//            if (getNumSubItems() == 0)
//            {
//                // create and add sub-items to this node of the tree, corresponding to
//                // each sub-element in the XML..
//                
//                forEachXmlChildElement (xml, child)
//                {
//                    jassert (child != 0);
//                    addSubItem (new TreeViewDemoItem (*child));
//                }
//            }
        }
        else
        {
            // in this case, we'll leave any sub-items in the tree when the node gets closed,
            // though you could choose to delete them if that's more appropriate for
            // your application.
        }
    }
    
//    TreeViewComponent* createItemComponent()
//    {
//        //return new TreeViewComponent(this, isLibrary);
//    }
//    
//    var getDragSourceDescription()
//    {
//        return "TreeView Items";
//    }
    
private:
    String playlistTitle;
    
    Image playlistIcon;
    Image libraryIcon;
    
    //LibraryTreeView* parent;
};

class LibraryTreeView : public Component
{
public:
    LibraryTreeView();
    ~LibraryTreeView();
    
    void paint(Graphics &g);
    void resized();
    
    void setSelected (String incomingPlaylist);
    Value getSelectedPlaylistValue();
    
private:
    TreeView treeView;
    ScopedPointer<LibraryViewItem> rootItem;
    
    Value selectedPlaylist;
};

#endif
