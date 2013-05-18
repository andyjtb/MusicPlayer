/*
  ==============================================================================
  
  This file is part of the dRowAudio JUCE module
  Copyright 2004-12 by dRowAudio.
  
  ------------------------------------------------------------------------------
 
  dRowAudio can be redistributed and/or modified under the terms of the GNU General
  Public License (Version 2), as published by the Free Software Foundation.
  A copy of the license is included in the module distribution, or can be found
  online at www.gnu.org/licenses.
  
  dRowAudio is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
  A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
  
  ==============================================================================
*/

#ifndef __DROWAUDIO_MUSICLIBRARYTABLE_H__
#define __DROWAUDIO_MUSICLIBRARYTABLE_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "ITunesLibrary.h"
#include "Comparators.h"
#include "MusicLibraryHelpers.h"
#include "Settings.h"
#include "TrackDialog.h"
#include "TrackMulti.h"

class GuiControl;

//==============================================================================
/**
    Table to display and interact with a music library.
    The easiest way to use this is to load a default or saved iTunes library like so:
    
    @code
        MusicLibraryTable table;
        table.setLibraryToUse (ITunesLibrary::getInstance());

        File parsedLibraryFile (File::getSpecialLocation (File::userDesktopDirectory).getChildFile ("saved_library_file.xml"));
        ValueTree libraryTree (readValueTreeFromFile (parsedLibraryFile));
 
        ITunesLibrary::getInstance()->setLibraryTree (libraryTree);
        ITunesLibrary::getInstance()->setLibraryFile (ITunesLibrary::getDefaultITunesLibraryFile());
    @endcode
*/
class MusicLibraryTable	: public Component,
                          public TableListBoxModel,
						  public ITunesLibrary::Listener,
						  public ActionBroadcaster,
                          public TextEditorListener
{
public:
    //==============================================================================
    /** Create the MusicLibraryTable.
     
        This will initially be blank, set an ITunesLibrary to use first with
        setLibraryToUse(). The table will then be continually updated as the library
        is parsed.
     */
    MusicLibraryTable();

    /** Destructor.
     */
    ~MusicLibraryTable();

    /** Sets the ITunesLibrary to use.
     */
	void setLibraryToUse (ITunesLibrary* library);

    /** Filters the table to only rows containing the given text.
     */
	void setFilterText (const String& filterText);
    
	/**	Returns the table list box component.
     */
	TableListBox& getTableListBox()	{	return table;	};
    
    //==============================================================================
    /** @internal */
	void libraryChanged (ITunesLibrary* library);
	
    /** @internal */
	void libraryUpdated (ITunesLibrary* library);
	
    /** @internal */
	void libraryFinished (ITunesLibrary* library);
	
    //==============================================================================
    /** Returns the number of rows currently bein displayed in the table.
     */
    int getNumRows();

    /** @internal */
    void paintRowBackground (Graphics& g, int rowNumber,
                             int width, int height, bool rowIsSelected);

    /** @internal */
    void paintCell (Graphics& g,
                    int rowNumber,
                    int columnId,
                    int width, int height,
                    bool rowIsSelected);

    /** @internal */
    void sortOrderChanged (int newSortColumnId, bool isForwards);

    /** @internal */
    int getColumnAutoSizeWidth (int columnId);
	
    //==============================================================================
    /** @internal */
    void resized();

    /** @internal */
	void focusOfChildComponentChanged (FocusChangeType cause); 

    /** @internal */
	var getDragSourceDescription (const SparseSet<int>& currentlySelectedRows);

    //NON DROW FUNCTIONS
    /** Sets the pointer to the GuiControl parent class */
    void setGuiControl(GuiControl* _guiControl) { guiControl.set(_guiControl, false); }
    /** Gets the pointer to the GuiControl parent class */
    GuiControl* getGuiControl() { return guiControl; }
    /** Calls the libraryUpdated function on this library - Wrapper class to allow other classes to call libraryUpdated without having a pointer to the iTunesLibrary in use */
    void updateLibrary();
    /** Updates the selectedTree and selectedRow. Sends an ActionMessage alerting other classes to this change, if multiple rows are selected, reads their unique ID numbers into an array */
    void selectedRowsChanged (int lastRowSelected);
    /** Plays the currently selected song when the return key is pressed */
    void returnKeyPressed (int currentSelectedRow);
    /** Calls deleteTracks when delete key is press */
    void deleteKeyPressed (int currentSelectedRow);
    /** Brings up the right click menu, also allows alt-click to edit directly. */
    void cellClicked (int rowNumber, int columnId, const MouseEvent &event);
    /** Plays the song that was double clicked */
    void cellDoubleClicked(int rowNumber, int columnId, const MouseEvent &event);
    
    /** Brings up a calloutBox allowing the user to directly edit the information selected */
    void editDirectly (int rowNumber, int columnId, const MouseEvent &event);
    /** Created as part of the editDirectly function - When enter is pressed the edit box the changes are saved and the box is closed */
    void textEditorReturnKeyPressed (TextEditor & editor);
    /** Created as part of the editDirectly function - When escape is pressed the box is closed and the changes are not saved */
    void textEditorEscapeKeyPressed(TextEditor &);
    
    /** Sets the playlist tree */
    void setPlaylistTree (ValueTree& playlist);
    /** Changes the ValueTree being displayed in the table - if displayPlaylists is true then it displays a playlist and changes the various right click options accordingly */
    void changeDisplay (bool displayPlaylists);
    /** Returns whether the table is displaying a playlist */
    bool isDisplayingPlaylist() { return displayPlaylist; }
    /** Returns whether a playlist is currently being played */
    bool isPlayingPlaylist() { return playingPlaylist; }
    /** External wrapper around the set sort column function in the contained table - Allows the sort column to be changed when a playlist is displayed, so the playlist is in the custom order set by the user */
    void setSortColumn (int columnNumber);
    
    /** Creates a window asking whether the user wants to delete the song from the library or the computer - if computer is selected the files are moved to the trash for safety */
    void deleteTracks (bool libraryOnly);
    
    /** Moves the selected song up or down in the playlist based upon the moveUp param - Rearranges songs and saves to playlist file */
    void playlistRearrange (int selectedRow, bool moveUp);
    
    /** Returns whether the table is currently deleting - Stops the information about a song being loaded whilst the table is deleting, speeding up the delete process dramatically */
    bool isTableDeleting() { return tableDeleting; }
    
    /** Returns the information about the track that is currently selected */
    ValueTree getCurrentlySelectedTree() { return currentlySelectedRow; }
    /** Returns the row that is currently selected */
    int getCurrentlySelectedRow() { return table.getSelectedRow(); }
    
    /** External wrapper around the selectRow function - Has default values to allow just the new row to be send to the function*/
    void setCurrentlySelectedRow(int selectRow, bool scrollToRow = false, bool deselectRows = true) { table.selectRow(selectRow, scrollToRow, deselectRows); }
    
    void updateTableFilteredAndSorted();
    
private:
    //==============================================================================
    Font font;	
	ITunesLibrary* currentLibrary;
    TableListBox table;
    String currentFilterText;
    
    OptionalScopedPointer<GuiControl> guiControl;
    
    ValueTree dataList, playlistTree;
    
    ScopedPointer<TrackDialog> trackDialog;
    
    OptionalScopedPointer<CallOutBox> callout;
    ValueTree currentlyEditing, currentlySelectedRow;
    ScopedPointer<TextEditor> editDirectlyText;
    
    int filteredNumRows, columnEditing, rowEditing;
	bool finishedLoading, displayPlaylist, playingPlaylist, tableDeleting;
	
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MusicLibraryTable);
};

#endif // __DROWAUDIO_MUSICLIBRARYTABLE_H__