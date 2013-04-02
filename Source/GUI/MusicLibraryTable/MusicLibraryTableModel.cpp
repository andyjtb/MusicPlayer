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

#include "MusicLibraryTableModel.h"

MusicLibraryTable::MusicLibraryTable()
:	font (12.0f),
currentLibrary (nullptr),
dataList (MusicColumns::libraryIdentifier),
filteredNumRows (dataList.getNumChildren()),
finishedLoading (true)
{
    displayPlaylist = false;
    
	// Create our table component and add it to this component..
	addAndMakeVisible (&table);
    table.setModel (this);
    table.setMultipleSelectionEnabled (true);
	table.setColour (ListBox::backgroundColourId, Colour::greyLevel (0.2f));
	table.setHeaderHeight (18);
	table.setRowHeight (16);
	table.getViewport()->setScrollBarThickness (10);
    
	// give it a border
	table.setColour (ListBox::outlineColourId, Colours::grey);
	table.setOutlineThickness (1);
    
	// Add some MusicColumns to the table header
    for (int i = 1; i < MusicColumns::numColumns; i++)
    {
        table.getHeader().addColumn (MusicColumns::columnNames[i].toString(),
                                     i,
                                     MusicColumns::columnWidths[i],
                                     50,
                                     800,
                                     TableHeaderComponent::defaultFlags);
    }
	// we could now change some initial settings..
	table.getHeader().setSortColumnId (MusicColumns::Artist, true); // sort forwards by the Artist column
    
    for (int i = 0; i < MusicColumns::numColumns; i++)
    {
        if (i == 3|| i==4 || i== 5 || i== 6 || i== 8 || i== 12 || i== 15 || i== 18)
            table.getHeader().setColumnVisible (i, true);
        else
            table.getHeader().setColumnVisible(i, false);
    }
    
	setFilterText (String::empty);
}

MusicLibraryTable::~MusicLibraryTable()
{
	if (currentLibrary != nullptr)
		currentLibrary->removeListener(this);
    
    if (!displayPlaylist)
        DBG(table.getHeader().toString());
}

void MusicLibraryTable::setLibraryToUse (ITunesLibrary* library)
{
	currentLibrary = library;
    //	DBG("ITunesLibrary to use changed");
	filteredDataList = dataList = library->getLibraryTree();
	dataList = library->getLibraryTree();
	library->addListener(this);
	libraryChanged(library);
}

void MusicLibraryTable::setFilterText (const String& filterString)
{
    currentFilterText = filterString;
    if (currentLibrary != nullptr)
        currentLibrary->getParserLock().enter();
    
	if (filterString == String::empty)
	{
		filteredDataList = dataList;
		filteredNumRows = filteredDataList.getNumChildren();
	}
	else
	{
		filteredDataList = ValueTree (dataList.getType());
		
		for (int e = 0; e < dataList.getNumChildren(); ++e)
		{
			for (int i = 0; i < dataList.getChild (e).getNumProperties(); i++)
			{
				if (dataList.getChild (e)[MusicColumns::columnNames[i]].toString().containsIgnoreCase (filterString))
				{
					filteredDataList.addChild (dataList.getChild(e).createCopy(), -1, 0);
					
					break;
				}
				
			}
		}
		
		filteredNumRows = filteredDataList.getNumChildren();
	}
	
    if (currentLibrary != nullptr)
        currentLibrary->getParserLock().exit();
    
	table.getHeader().reSortTable();
	table.updateContent();
}

//==============================================================================
void MusicLibraryTable::libraryChanged (ITunesLibrary* library)
{
	if (library == currentLibrary)
	{
        //DBG("Library changed");
        //NON DROW
        if(displayPlaylist)
        {
            filteredDataList = dataList = playlistTree.getChildWithName("TrackInfo");
        }
        else
            filteredDataList = dataList = currentLibrary->getLibraryTree();
        
		filteredNumRows = filteredDataList.getNumChildren();
        
		finishedLoading = false;
        updateTableFilteredAndSorted();
    }
}

void MusicLibraryTable::libraryUpdated (ITunesLibrary* library)
{
	if (library == currentLibrary)
	{
        if(remoteConnections.getFirst() != nullptr)
        {
            //remoteConnections.getFirst()->sendLibraryData();
        }
        
        updateTableFilteredAndSorted();
	}
}

void MusicLibraryTable::libraryFinished (ITunesLibrary* library)
{
	if (library == currentLibrary)
	{
		sendActionMessage("LibraryImportFinished");
        updateTableFilteredAndSorted();
	}
}

//==============================================================================
int MusicLibraryTable::getNumRows()
{
	return filteredNumRows;
}

void MusicLibraryTable::paintRowBackground (Graphics& g, int /*rowNumber*/,
                                            int /*width*/, int /*height*/, bool rowIsSelected)
{
	if (rowIsSelected)
		table.hasKeyboardFocus (true) ? g.fillAll (Colours::darkorange) : g.fillAll (Colour::greyLevel (0.6f));
	else
		g.fillAll (Colour::greyLevel (0.2f));
}

void MusicLibraryTable::paintCell (Graphics& g,
								   int rowNumber,
								   int columnId,
								   int width, int height,
								   bool rowIsSelected)
{
	rowIsSelected ? g.setColour (Colour::greyLevel (0.2f)) : g.setColour (Colour::greyLevel (0.9f));
	g.setFont (font);
    
    {
        ScopedLock sl (currentLibrary->getParserLock());
        const ValueTree& rowElement (filteredDataList.getChild (rowNumber));
        
        if (rowElement.isValid())
        {
            String text;
            
            if(columnId == MusicColumns::Length)
                text = TimeHelpers::secondsToTimeLength (rowElement[MusicColumns::columnNames[columnId]].toString().getIntValue());
            else if(columnId == MusicColumns::Added
                    || columnId == MusicColumns::Modified)
                text = Time (int64 (rowElement[MusicColumns::columnNames[columnId]])).formatted ("%d/%m/%Y - %H:%M");
            else if(columnId == MusicColumns::Size)
                text = File::descriptionOfSizeInBytes(rowElement[MusicColumns::columnNames[columnId]]);
            else if(columnId == MusicColumns::BitRate)
                text = String(rowElement[MusicColumns::columnNames[columnId]].toString()+"kbps");
            else
                text = rowElement[MusicColumns::columnNames[columnId]].toString();
            
            g.drawText (text, 2, 0, width - 4, height, Justification::centredLeft, true);
        }
    }
    
	g.setColour (Colour::greyLevel (0.9f).withAlpha (0.2f));
	g.fillRect (width - 1, 0, 1, height);
	g.fillRect (0, height - 1, width, 1);
}

void MusicLibraryTable::sortOrderChanged (int newSortColumnId, bool isForwards)
{
	if (newSortColumnId != 0)
	{
        ScopedLock sl (currentLibrary->getParserLock());
        
		if (newSortColumnId == MusicColumns::Length
			|| newSortColumnId == MusicColumns::BPM
			|| newSortColumnId == MusicColumns::LibID
			|| newSortColumnId == MusicColumns::ID
            || newSortColumnId == MusicColumns::Added
            || newSortColumnId == MusicColumns::Modified
            || newSortColumnId == MusicColumns::Size)
		{
			ValueTreeComparators::Numerical sorter (MusicColumns::columnNames[newSortColumnId], isForwards);
			filteredDataList.sort (sorter, 0, false);
		}
		else
        {
//			ValueTreeComparators::LexicographicWithBackup sorter (MusicColumns::columnNames[newSortColumnId],
//                                                                  MusicColumns::columnNames[MusicColumns::LibID],
//                                                                  isForwards);
            ValueTreeComparators::LexicographicWithNumerical sorter (MusicColumns::columnNames[newSortColumnId],
                                                                    MusicColumns::columnNames[MusicColumns::LibID],
                                                                    isForwards);
            
			filteredDataList.sort (sorter, 0, false);
		}

		table.updateContent();
	}
}

//==========================================================================================
int MusicLibraryTable::getColumnAutoSizeWidth (int columnId)
{
	int widest = 32;
    
	// find the widest bit of text in this column..
	for (int i = getNumRows(); --i >= 0;)
	{
        {
            ScopedLock sl (currentLibrary->getParserLock());
            const ValueTree& rowElement (filteredDataList.getChild (i));
            
            if (rowElement.isValid())
            {
                const String text (rowElement[MusicColumns::columnNames[columnId]].toString());
                widest = jmax (widest, font.getStringWidth (text));
            }
        }
	}
    
	return widest + 8;
}

//==============================================================================
void MusicLibraryTable::resized()
{
	table.setBounds (getLocalBounds());
}

void MusicLibraryTable::focusOfChildComponentChanged (FocusChangeType /*cause*/)
{
	repaint();
}

//NON DROW
var MusicLibraryTable::getDragSourceDescription (const SparseSet< int > &currentlySelectedRows)
{

	if(! currentlySelectedRows.isEmpty())
	{
        Array<var> itemsArray;
        
        for (int i = 0; i < currentlySelectedRows.size(); ++i)
        {
            ValueTree currentRow = filteredDataList.getChild(currentlySelectedRows[i]);
            itemsArray.add(currentRow.getProperty(MusicColumns::columnNames[MusicColumns::ID]));
        }
        
        return var(itemsArray);
        
	}
    
	return var::null;
}

//==============================================================================
void MusicLibraryTable::updateTableFilteredAndSorted()
{
    // make sure we still apply our filter
    // this will also re-sort and update the table
    setFilterText (currentFilterText);
}

//NON DROW
void MusicLibraryTable::updateLibrary()
{
    libraryUpdated(currentLibrary);
}

void MusicLibraryTable::selectedRowsChanged(int lastRowSelected)
{
    tableSelectedRow = filteredDataList.getChild(lastRowSelected);
    
    SparseSet<int> selectedRows = table.getSelectedRows();
    tableSelectedTracks.clear();
    
    for (int counter = 0; counter < selectedRows.size(); counter++) {
        tableSelectedTracks.add(filteredDataList.getChild(selectedRows[counter]).getProperty(MusicColumns::columnNames[MusicColumns::ID]));
    } 
    
    sendActionMessage("SelectedRowsChanged");
}

void MusicLibraryTable::returnKeyPressed(int currentSelectedRow)
{
    tableShouldPlay.setValue(true);
    tableSelectedRow = filteredDataList.getChild(currentSelectedRow);
}

void MusicLibraryTable::deleteKeyPressed(int currentSelectedRow)
{
    if (displayPlaylist) {
        deleteTracks(true);
    }
    else
    {
        SparseSet<int> selectedRows = table.getSelectedRows();
        
        String deleteString = "Delete ";
        deleteString << selectedRows.size();
        
        String trackString;
        selectedRows.size() > 1 ? trackString << " tracks?" : trackString << " track?";
        deleteString << trackString;
        
        String longString = "Are you sure you wish to delete ";
        longString << selectedRows.size() << trackString << "\nYou may chose to delete from the library only or from your computer permanently";
        
        
        AlertWindow deletePopup(deleteString, longString, AlertWindow::WarningIcon);
        
        deletePopup.addButton("Cancel", 0);
        deletePopup.addButton("Library", 1);
        deletePopup.addButton("Computer and library", 2);
        
        int result = deletePopup.runModalLoop();
        
        if (result == 1)
        {
            deleteTracks(true);
        }
        else if (result == 2)
        {
            deleteTracks(false);
        }
    }
}

void MusicLibraryTable::deleteTracks (bool libraryOnly)
{
    
    tableDeleting = true;
    //Add a confirmation screen containing the option to delete file as well
    //Removing this section seems sensible as tableSelectedTracks does the same, but if used it only deletes half of the selection at a time
    
    SparseSet<int> selectedRows = table.getSelectedRows();
    Array<int> toDelete;
    
    for (int counter = 0; counter < selectedRows.size(); counter++) {
        toDelete.add(filteredDataList.getChild(selectedRows[counter]).getProperty(MusicColumns::columnNames[MusicColumns::ID]));
    } 
    
    String failedToDelete = "Could not delete: ";
    
    for (int counter = 0; counter < toDelete.size(); counter++)
    {
        ValueTree valueDelete = filteredDataList.getChildWithProperty(MusicColumns::columnNames[MusicColumns::ID], toDelete[counter]);
        
        
        if (tablePlayingRow == valueDelete)
        {
            singletonPlayState = false;
        }
        
        if (!libraryOnly) {
            File fileToDelete = valueDelete.getProperty(MusicColumns::columnNames[MusicColumns::Location]).toString();
            bool result = fileToDelete.moveToTrash();
            
            if (!result)
                failedToDelete << fileToDelete.getFileName() << "\n";
        }
        
        //Move this line to within the libraryonly if statement and remove undomanager to make sure they can't undo until you work out how to get something back from the trash
        filteredDataList.removeChild(valueDelete, singletonUndoManager);
        
    }
    
    //If deleting files
    if (!libraryOnly)
    {
        //If anything has been added to the failed to delete string then something has failed
        if (failedToDelete!= "Could not delete: ") 
            AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Failed", failedToDelete);
        
        else
            AlertWindow::showMessageBox(AlertWindow::InfoIcon, "Success", "All files successfully moved to the trash");
    }
    
    
    if (displayPlaylist)
    {
        String playlistName = playlistTree.getProperty("Name").toString();
        int playlistId = playlistTree.getProperty("PlaylistID");
        playlistTree.removeAllProperties(0);
        
        playlistTree.setProperty("Name", playlistName, 0);
        playlistTree.setProperty("PlaylistID", playlistId, 0);
        
        playlistTree.setProperty("Modified", Time::getCurrentTime().getMilliseconds(), 0);
        
        int size = 1;
        
        for (int i = 0; i < filteredDataList.getNumChildren(); i++) {
            String trackID ("TrackID");
            trackID << size++;
            playlistTree.setProperty(trackID, filteredDataList.getChild(i).getProperty("ID"), 0);
        }
        
        playlistTree.setProperty("Size", size, 0);
    }
    
    
    //DBG("Trans num = " << singletonUndoManager->getNumActionsInCurrentTransaction());
    //DBG("Undo message = " << singletonUndoManager->getUndoDescription());
    tableUpdateRequired = true;
    tableDeleting = false;
    
    table.deselectAllRows();
    
    if (selectedRows[0] == 0 || selectedRows[0] == (table.getNumRows()-1))
        table.selectRow(selectedRows[0]);
    else 
        table.selectRow(selectedRows[0]-1);
    
    if(filteredDataList.getNumChildren() > 0)
    {
        tableSelectedRow = filteredDataList.getChild(table.getSelectedRow());
        tableLoadSelected = true;
    }
    
    updateTableFilteredAndSorted();
}

void MusicLibraryTable::cellClicked(int rowNumber, int columnId, const juce::MouseEvent &event)
{
    if (event.mods.isPopupMenu()) {
        PopupMenu rightClick;
        if (columnId == MusicColumns::Artist ||
            columnId == MusicColumns::Song ||
            columnId == MusicColumns::Album ||
            columnId == MusicColumns::Rating ||
            columnId == MusicColumns::Genre ||
            columnId == MusicColumns::Score) {
            String editMessage("Edit ");
            editMessage << MusicColumns::columnNames[columnId].toString();
            rightClick.addItem(1, editMessage);
            rightClick.addSeparator();
        }
        
        rightClick.addItem(2, "Play");
		rightClick.addItem(3, "Display Info");
        rightClick.addItem(4, "Show in Finder");
        rightClick.addSeparator();
        
        if (displayPlaylist)
        {
            rightClick.addItem(6, "Move Up");
            rightClick.addItem(7, "Move Down");
            rightClick.addSeparator();
        }
        
        rightClick.addItem(5, "Delete Song");
		
        int result = rightClick.show();
        
        switch (result) {
            case 1:
            {
                editDirectly(rowNumber, columnId);
                break;
            }
            case 2:
            {
                tableShouldPlay.setValue(true);
                tableSelectedRow = filteredDataList.getChild(rowNumber);
                break;
            }
            case 3:
            {
                SparseSet<int> selectedRows = table.getSelectedRows();
                
                if (selectedRows.size() == 1)
                {
                    ValueTree currentlyEditing = filteredDataList.getChild(rowNumber);
                    File test = currentlyEditing.getProperty(MusicColumns::columnNames[MusicColumns::Location]).toString();
                    
                    if (test.exists())
                    {
                        trackDialog = new TrackDialog(rowNumber);             
                    
                        DialogWindow::showDialog(filteredDataList.getChild(rowNumber).getProperty(MusicColumns::columnNames[MusicColumns::Song]), trackDialog, 0, Colours::white, true);
                    }
                }
                else
                {
                    SparseSet<int> selectedRows = table.getSelectedRows();
                    Array<int> editingIds;
                    
                    for (int counter = 0; counter < selectedRows.size(); counter++) {
                        ValueTree currentlyEditing = filteredDataList.getChild(selectedRows[counter]);
                        File test = currentlyEditing.getProperty(MusicColumns::columnNames[MusicColumns::Location]).toString();
                        
                        if (test.exists())
                        {
                            editingIds.add(currentlyEditing.getProperty(MusicColumns::columnNames[MusicColumns::LibID]));
                        }
                    } 
                    
                    AlertWindow editPopup("Editing Multiple Tracks", "", AlertWindow::NoIcon);
                    ScopedPointer<TrackMulti> trackMulti;
                    trackMulti = new TrackMulti(editingIds);
                    
                    editPopup.addCustomComponent(trackMulti);
                    
                    editPopup.addButton("Cancel", 0);
                    editPopup.addButton("Ok", 1);
                    
                    if (editPopup.runModalLoop() != 0) {
                        trackMulti->saveEdits();
                    }
                    editPopup.removeCustomComponent(0);
                }
                break;
            }
            case 4:
            {
                File(filteredDataList.getChild(rowNumber).getProperty(MusicColumns::columnNames[MusicColumns::Location])).revealToUser();
                break;
            }
            case 5:
            {
                deleteKeyPressed(rowNumber);
                break;
            }
                
            case 6:
            {
                playlistRearrange(rowNumber, true);
                break;
            }
            
            case 7:
            {
                playlistRearrange(rowNumber, false);
                break;
            }
                
            default:
            {
                break;
            }
        }
    }
    if (event.mods.isAltDown())
    {
        editDirectly(rowNumber, columnId);
    }
}

void MusicLibraryTable::cellDoubleClicked(int rowNumber, int columnId, const juce::MouseEvent &event)
{
    tableShouldPlay = true;
    tableSelectedRow = filteredDataList.getChild(rowNumber);
}

void MusicLibraryTable::editDirectly (int rowNumber, int columnId)
{
    ValueTree currentlyEditing = filteredDataList.getChild(rowNumber);
    File test = currentlyEditing.getProperty(MusicColumns::columnNames[MusicColumns::Location]).toString();
    
    if (test.exists())
    {
        String popupTitle = ("Edit " + MusicColumns::columnNames[columnId].toString()+"...");
        String info = ("Editing: \n" + currentlyEditing.getProperty(MusicColumns::columnNames[MusicColumns::Song]).toString() + " by " + currentlyEditing.getProperty(MusicColumns::columnNames[MusicColumns::Artist]).toString());
        AlertWindow editPopup(popupTitle, info, AlertWindow::NoIcon);
        TextEditor editText;
        
        editText.setBounds(getWidth()/2, getHeight()/2, 300, 25);
        editText.setText(currentlyEditing.getProperty(MusicColumns::columnNames[columnId]));
        
        editPopup.addCustomComponent(&editText);
        
        editPopup.addButton("Cancel", 0);
        editPopup.addButton("Ok", 1);
        
        if (editPopup.runModalLoop() != 0) {
            currentlyEditing.setProperty(MusicColumns::columnNames[columnId], editText.getText(), singletonUndoManager);
            TagReader::writeTag(columnId, currentlyEditing);
        }
        editPopup.removeCustomComponent(0);
    }
}

void MusicLibraryTable::setPlaylistTree (ValueTree& playlist)
{
    playlistTree = playlist;
}

void MusicLibraryTable::changeDisplay(bool displayPlaylists)
{
    displayPlaylist = displayPlaylists;
    libraryChanged (currentLibrary);
}

void MusicLibraryTable::setSortColumn (int columnNumber)
{
    table.getHeader().setSortColumnId(columnNumber, true);
}

void MusicLibraryTable::playlistRearrange (int selectedRow, bool movingUp)
{
    Identifier libID = "LibID";
    ValueTree movingRow = filteredDataList.getChild(selectedRow);
    if (movingUp)
        selectedRow--;
    else
        selectedRow++;
    
    ValueTree otherRow = filteredDataList.getChild(selectedRow);
    
    if (otherRow.isValid())
    {
        int movingID = movingRow.getProperty(libID);
        int otherID = otherRow.getProperty(libID);
        movingRow.setProperty(libID, otherID, 0);
        otherRow.setProperty(libID, movingID, 0);
    }
    
    updateTableFilteredAndSorted();
}
