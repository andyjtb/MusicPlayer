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
/** Each item in the LibraryTreeView */
class LibraryViewItem  : public TreeViewItem,
                         public ActionBroadcaster,
                         public TextEditor::Listener
{
public:
    /** Constructor - Takes the playlists title as a string */
    LibraryViewItem (String incomingPlaylistTitle)
    {
        //parent = incomingParent;
        
        libraryIcon = ImageCache::getFromMemory (LibraryTreeViewBinaryData::small_library_png, LibraryTreeViewBinaryData::small_library_pngSize);
        playlistIcon = ImageCache::getFromMemory (LibraryTreeViewBinaryData::small_playlist_png, LibraryTreeViewBinaryData::small_playlist_pngSize);

        playlistTitle = incomingPlaylistTitle;
        
        setLinesDrawnForSubItems(false);  
    }
    /** Destuctor */
    ~LibraryViewItem()
    {
        callout.clear();
    }
    /** Called when an item is selected. 
     If isNowSelected is true sends a message saying it has been selected */
    void itemSelectionChanged(bool isNowSelected)
    {
        if (isNowSelected)
        {
            sendActionMessage("Playlist Changed: " + playlistTitle);
        }
    }
    /** Returns pre defined item width */
    int getItemWidth() const
    {
        return 150;
    }
    /** Returns pre define item height */
    int getItemHeight() const
    {
        return 30;
    }
    /** Returns the playlists title */
    String getUniqueName() const
    {
        return String(playlistTitle);
    }
    /** Returns false */
    bool mightContainSubItems()
    {
        return 0;//xml->getFirstChildElement() != 0;
    }
    /** Draws the item - title and icon, also draws highlight overlay when selected */
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
    /** @internal */
    void itemOpennessChanged (bool isNowOpen)
    {
    }
    /** Allows songs to be dragged to the playlist item in the LibraryTreeView */
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
    /** Adds the songs dropped to the playlist */
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
    /** Allows renaming through the right click or alt-click, allows deleting through right click */
    void itemClicked (const MouseEvent& e)
    {
        if (playlistTitle != "Library")
        {
            if (e.mods.isPopupMenu())
            {
                PopupMenu rightClick;
                
                rightClick.addItem(1, "Rename Playlist");
                rightClick.addSeparator();
                rightClick.addItem(2, "Delete Playlist");
                
                if (rightClick.show() != 1)
                {
                    String deleteString = "Delete " + playlistTitle;
                    
                    String longString = "Are you sure you wish to delete ";
                    longString << playlistTitle << "?";
                    
                    AlertWindow deletePopup(deleteString, longString, AlertWindow::WarningIcon);
                    
                    deletePopup.addButton("Cancel", 0);
                    deletePopup.addButton("Ok", 1);
                    
                    if (deletePopup.runModalLoop() != 0)
                    {
                        ValueTree toDelete = singletonPlaylistsTree.getChildWithProperty(MusicColumns::playlistName, playlistTitle);
                        if (toDelete.isValid())
                            singletonPlaylistsTree.removeChild(toDelete, 0);
                        sendActionMessage("UpdateItems");
                    }
                    
                }
                else
                {
                    rename(e);
                }
                
            }
            else if (e.mods.isAltDown())
            {
                rename(e);
            }
        }
    }
    /** Displays a box allowing the playlist to be renamed */
    void rename(const MouseEvent& e)
    {
        Rectangle<int> position;
        position.setPosition(e.getScreenPosition());
        
        editDirectlyText = new TextEditor();
        editDirectlyText->addListener(this);
        editDirectlyText->setSize(300,25);
        editDirectlyText->setText(playlistTitle);
        
        callout.set(new CallOutBox(*editDirectlyText, position, nullptr), false);
        callout->enterModalState(true, nullptr, true);
    }
    /** Saves the new playlist name is the user presses the enter key whilst renaming */
    void textEditorReturnKeyPressed (TextEditor &editor)
    {
        ValueTree rename = singletonPlaylistsTree.getChildWithProperty(MusicColumns::playlistName, playlistTitle);
        if (rename.isValid())
        {
            rename.setProperty(MusicColumns::playlistName, editor.getText(), 0);
            int idNum = rename.getProperty(MusicColumns::playlistID);
            idNum += 200;
            rename.setProperty(MusicColumns::playlistID, idNum, 0);
            
            sendActionMessage("UpdateItems");
        }
        
        callout->exitModalState(0);
    }
    /** Hides rename box is the user presses escape whilst renaming */
    void textEditorEscapeKeyPressed(TextEditor &)
    {
        callout->exitModalState(0);
    }
    
private:
    String playlistTitle;
    
    Image playlistIcon;
    Image libraryIcon;
    
    OptionalScopedPointer<CallOutBox> callout;
    ScopedPointer<TextEditor> editDirectlyText;
    //LibraryTreeView* parent;
};
/** A TreeView displaying the library and the playlists in a user's library. Allows them to be selected
 @see LibraryViewItem */
class LibraryTreeView : public Component,
                        public ActionListener,
                        public ChangeBroadcaster,
                        public Button::Listener
{
public:
    LibraryTreeView();
    ~LibraryTreeView();
    
    /** Reloads all LibraryTreeViewItem s */
    void updateItems();
    /** @internal */
    void paint(Graphics &g);
    /** @internal */
    void resized();
    
    /** Sets selectedPlaylist to incomingPlaylist */
    void setSelected (String incomingPlaylist);
    /** Gets the selectedPlaylist name */
    String getSelectedPlaylist();
    /** Controls loading new playlists and updateing items when the action message is received */
    void actionListenerCallback (const String& message);
    /** Button listener for add or remove playlist buttons */
    void buttonClicked (Button* button);
    
    /** Fills a ValueTree with the full information for a playlist's tracks based on the trackID numbers for each song */
    void loadPlaylistsTrackInfo();
    
private:
    TreeView treeView;
    ScopedPointer<LibraryViewItem> rootItem;
    
    String selectedPlaylist;
    
    TextButton plusButton, minusButton;
};

#endif
