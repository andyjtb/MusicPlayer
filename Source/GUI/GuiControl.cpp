/*
 *  GuiControl.cpp
 *  MusicPlayer
 *
 *  Created by Andy on 26/10/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "GuiControl.h"
#include "RemoteInterprocessConnection.h"

GuiControl::GuiControl() : playbackControl(this)
{	
	addAndMakeVisible(&playbackControl);
	
	addAndMakeVisible(&volumeControl);
	volumeControl.addValueListener(this);
	
	addAndMakeVisible(&outputMeters);
	
	addAndMakeVisible(&transport);
	transport.addActionListener(this);
	
	addAndMakeVisible(&trackInfo);
	
	addAndMakeVisible(&albumArt);
    
    addAndMakeVisible(&infoBar);
	
    addAndMakeVisible(&searchBox);
    searchBox.getSearchTextEditor().addListener(this);
    
	ITunesLibrary::getInstance()->setLibraryTree (singletonLibraryTree);
    ITunesLibrary::getInstance()->setPlaylistsTree(singletonPlaylistsTree);
    musicTable = musicLibraryDropTarget.getMusicTable();
	musicTable->setLibraryToUse (ITunesLibrary::getInstance());
	musicTable->addActionListener(this);
    musicTable->setGuiControl (this);
    
    tableUpdateRequired.addListener(this);
    
    addAndMakeVisible(&musicLibraryDropTarget);
    
    addAndMakeVisible(&libraryView);
    libraryView.addChangeListener(this);
//    coverflow = new CoverFlowComponent();
//    addAndMakeVisible(coverflow);
//    setSize(500, 400);
    singletonPlayState.addListener(this);
    artUpdateRequired.addListener(this);
}

GuiControl::~GuiControl()
{
	
}

void GuiControl::paint(Graphics& g)
{
//    g.fillAll (Colour (0xff323541));
//    
//    g.setGradientFill (ColourGradient (Colour (0x30ffffff),
//                                       0.0f, 0.0f,
//                                       Colour (0x6e000000),
//                                       0, getHeight(),
//                                       false));
//    g.fillRect (0, 0, getWidth(), getHeight());    
}

void GuiControl::resized()
{
    playbackControl.setBounds(10, 5, 200, 65);
	volumeControl.setBounds(10, 90, 380, 80);
	outputMeters.setBounds(400, 90, 250, 80);
	transport.setBounds(playbackControl.getWidth() - 50, albumArt.getHeight()+10, 300, 60);
    
	trackInfo.setBounds(400, 6, 300, 70);
	albumArt.setBounds(220,5,175,175);
    
    infoBar.setBounds(0, getHeight()-15, getWidth(), 15);
    
	searchBox.setBounds(getWidth()-200, 0, 175, 60);

    musicLibraryDropTarget.setBounds(0, getHeight()/2-30, getWidth(), (getHeight()/2+30)-15);

    libraryView.setBounds(getWidth()-205, 50, 200, 200);
}

void GuiControl::setAudioControl(AudioControl* incomingAudioControl)
{
    audioControl = incomingAudioControl;
	audioControl->addChangeListener(this);
} 

void GuiControl::timerCallback(int timerId)
{
	if(timerId == 0)
	{
		float localOutputMeterL = audioControl->getOutputMeterValue("L");
		float localOutputMeterR = audioControl->getOutputMeterValue("R");
		outputMeters.setOutputMeterValueL(localOutputMeterL);
		outputMeters.setOutputMeterValueR(localOutputMeterR);
	}
	if (timerId == 1) {
        
		transport.setTransportPosition (audioControl->getTransportPosition());
        if(remoteConnections.getFirst() != nullptr)
        {
            remoteConnections.getFirst()->sendPosition(audioControl->getTransportPosition());
        }
        
        if (audioControl->getTransportPosition() >= transport.getMaximum())
        {
            stopTimer(0);
            stopTimer(1);
            
            int playCount = tablePlayingRow.getProperty(MusicColumns::columnNames[MusicColumns::PlayCount], 0);
            playCount++;
            tablePlayingRow.setProperty(MusicColumns::columnNames[MusicColumns::PlayCount], playCount, 0);
            
            String name = tablePlayingRow.getProperty("Song");
            
            musicTable->updateTableFilteredAndSorted();
            
            next();
        }
	}
}

void GuiControl::actionListenerCallback (const String& message)
{
    if (message == "LibraryImportFinished") {
		DBG("library Loaded");
        
        //Sort trees to make sure the final lib id is the highest
        
        ValueTreeComparators::Numerical librarySorter (MusicColumns::columnNames[MusicColumns::LibID], true);
        singletonLibraryTree.sort (librarySorter, 0, false);
        
        ValueTreeComparators::Numerical playlistSorter (MusicColumns::playlistID, true);
        singletonPlaylistsTree.sort (playlistSorter, 0, false);
        
        singletonCurrentLibId = singletonLibraryTree.getChild(singletonLibraryTree.getNumChildren()-1).getProperty(MusicColumns::columnNames[MusicColumns::LibID]);
        DBG("Current lib id = " << singletonCurrentLibId);
        singletonCurrentValueTreeId = singletonLibraryTree.getChild(singletonLibraryTree.getNumChildren()-1).getProperty(MusicColumns::columnNames[MusicColumns::ID]);
        
        singletonCurrentPlaylistId = singletonPlaylistsTree.getChild(singletonPlaylistsTree.getNumChildren()-1).getProperty(MusicColumns::playlistID);
        
        DBG("Library file = " << singletonLibraryFile.getFullPathName());
        DBG("Playlist file = " << singletonPlaylistsFile.getFullPathName());
        
        Settings::getInstance()->saveSingletons();
		ITunesLibrary::getInstance()->saveLibrary();
		singletonLibraryTree = ITunesLibrary::getInstance()->getLibraryTree();
		ITunesLibrary::getInstance()->setLibraryTree(singletonLibraryTree);
        
        musicTable->setFilterText(String::empty);
        libraryView.updateItems();
	}
    
	if (message.startsWith("transportPosition")) 
    {
        String subString = message.fromFirstOccurrenceOf (":", false, true);
        double value = subString.getDoubleValue();
		
		audioControl->setTransportPosition(value);
    }
    
    if (message.startsWith("SelectedRows"))
    {
        infoBar.updateBar();
        if (tablePlayingRow != musicTable->getCurrentlySelectedTree())
            updateSelectedDisplay();
    }
}

void GuiControl::changeListenerCallback (ChangeBroadcaster* changeBroadcaster)
{
    //New song loaded change message
	if (changeBroadcaster == audioControl) {
        audioControl->setVolume(volumeControl.getVolume());
        if(remoteConnections.getFirst() != nullptr)
        {
            remoteConnections.getFirst()->sendPlayingData();
        }
        
        transport.setMaximum(audioControl->getTransportLength());
	}
    
    if (changeBroadcaster == &libraryView)
    {
        if (libraryView.getSelectedPlaylist() == "Library")
        {
            musicTable->changeDisplay(false);
            musicTable->setSortColumn(MusicColumns::Artist);
            int index = filteredDataList.indexOf(filteredDataList.getChildWithProperty("ID", tablePlayingRow.getProperty("ID")));
            musicTable->getTableListBox().selectRow(index);
        }
        else
        {
            ValueTree playlistValueTree = singletonPlaylistsTree.getChildWithProperty("Name", libraryView.getSelectedPlaylist());
            
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
                
                musicTable->setPlaylistTree (playlistValueTree);
                musicTable->changeDisplay(true);
                musicTable->setSortColumn(MusicColumns::LibID);
                
                int index = filteredDataList.indexOf(filteredDataList.getChildWithProperty("ID", tablePlayingRow.getProperty("ID")));
                musicTable->getTableListBox().selectRow(index);
            }
            
        }
        
    }
}


void GuiControl::valueChanged (Value& valueChanged)
{
	if (valueChanged == volumeControl.getSliderValue()) {
		audioControl->setVolume(valueChanged.getValue());
        if(remoteConnections.getFirst() != nullptr)
        {
            remoteConnections.getFirst()->sendVolume(valueChanged.getValue());
        }
	}
    
    if (valueChanged == tableUpdateRequired)
    {
        if (tableUpdateRequired.getValue()) {
            musicTable->updateLibrary();
            musicTable->setFilterText(searchBox.getText());
            //CLEAR TRACK INFO DISPLAY
            tableUpdateRequired.setValue(false);
        }
    }
    
    if (valueChanged == singletonPlayState) {

        if(remoteConnections.getFirst() != nullptr)
        {
            remoteConnections.getFirst()->sendPlayState();
        }
        
        setVolume(volumeControl.getVolume());
        if(remoteConnections.getFirst() != nullptr)
        {
            remoteConnections.getFirst()->sendVolume(volumeControl.getVolume());
        }
        
		if (singletonPlayState.getValue()) {
            startTimer(0, 50);
			startTimer(1, 100);
		}
		else {
            stopTimer(1);
		}
        //iTunes feature, pausing or playing changes the view to the currently playing track
        int index = filteredDataList.indexOf(filteredDataList.getChildWithProperty("ID", tablePlayingRow.getProperty("ID")));
        if (index != -1)
            musicTable->getTableListBox().selectRow(index, false, true);
        //musicTable->getTableListBox().selectRow(filteredDataList.indexOf(tablePlayingRow));
	}
    
    if (valueChanged == artUpdateRequired)
    {
        if(artUpdateRequired.getValue())
        {
            File audioFile = musicTable->getCurrentlySelectedTree().getProperty("Location").toString();
            ImageWithType currentCover = TagReader::getAlbumArt(audioFile);
            albumArt.setImageOnly(currentCover);
            artUpdateRequired = false;
        }
    }
}
void GuiControl::loadFile(ValueTree treeToLoad, bool shouldPlay)
{
    File selectedFile (treeToLoad.getProperty(MusicColumns::columnNames[MusicColumns::Location]));

    DBG("Gui load = " << selectedFile.getFileName());
    
    if (selectedFile.existsAsFile())
    {
        int result = 0;
        
        Identifier id = MusicColumns::columnNames[MusicColumns::ID];
        
        singletonPlayState = false;
        result = audioControl->loadFile(selectedFile);

        if (result != 2)
        {
            tablePlayingRow = treeToLoad;
            
            if (treeToLoad.getProperty(id) != musicTable->getCurrentlySelectedTree().getProperty(id))
            {
                int index = filteredDataList.indexOf(filteredDataList.getChildWithProperty(id, treeToLoad.getProperty(id)));
                if (index != -1)
                    musicTable->getTableListBox().selectRow(index, false, true);
            }
            
            if (shouldPlay)
                singletonPlayState = true;
            
            trackInfo.loadTrackInfo(tablePlayingRow);
            artUpdateRequired = true;
        }

    }
    else
    {
        //File not found
        infoBar.displayFileStatus(selectedFile, 1);
    }
    
}

void GuiControl::updateSelectedDisplay()
{
    if (musicTable->isTableDeleting() != true)
    {
        File selectedFile (musicTable->getCurrentlySelectedTree().getProperty(MusicColumns::columnNames[MusicColumns::Location]));
        
        if (selectedFile.existsAsFile())
        {
            infoBar.displayFileStatus(selectedFile, 0);
            albumArt.setCover(selectedFile);
        }
        else
        {
            infoBar.displayFileStatus(selectedFile, 1);
            albumArt.setCover(Image());
        }
    }
}

void GuiControl::next()
{
    if (tablePlayingRow.isValid()) {
        Identifier id = MusicColumns::columnNames[MusicColumns::ID];
        
//Updates the list to allow for reordering playlists or changing the search - Causes crash when next is pressed a lot in quick sucession
//        if (currentlyPlayingName == libraryView.getSelectedPlaylist())
////            //currentlyPlayingList = filteredDataList.createCopy();
//            currentlyPlayingList = filteredDataList;
//        else
//            currentlyPlayingList = filteredDataList.createCopy();
        
        int toPlay = currentlyPlayingList.indexOf(currentlyPlayingList.getChildWithProperty(id, tablePlayingRow.getProperty(id)));
        
        ++toPlay;
        if (toPlay < currentlyPlayingList.getNumChildren())
        {
            //valid next song
            //Currently playing list is also the one being viewed, so move selectedrow
//            if (currentlyPlayingList.isEquivalentTo(filteredDataList))
//                musicTable->setCurrentlySelectedRow(toPlay, true);
        
            if (singletonPlayState.getValue())
                loadFile(currentlyPlayingList.getChild(toPlay), true);
            else
                loadFile(currentlyPlayingList.getChild(toPlay), false);
        }
        else
        {
                singletonPlayState = false;
        }
    }
}

void GuiControl::previous()
{
    if (tablePlayingRow.isValid()) {
        Identifier id = MusicColumns::columnNames[MusicColumns::ID];
        
        //Updates the list to allow for reordering playlists or changing the search
        if (currentlyPlayingName == libraryView.getSelectedPlaylist())
            currentlyPlayingList = filteredDataList.createCopy();
        
        int toPlay = currentlyPlayingList.indexOf(currentlyPlayingList.getChildWithProperty(id, tablePlayingRow.getProperty(id)));

        --toPlay;
        if (toPlay >= 0)
        {
//            if (currentlyPlayingList.isEquivalentTo(filteredDataList))
//                musicTable->setCurrentlySelectedRow(toPlay, true);
            
            if (singletonPlayState.getValue())
                loadFile(currentlyPlayingList.getChild(toPlay), true);
            else
                loadFile(currentlyPlayingList.getChild(toPlay), false);
        }
    }
}

//Remote
void GuiControl::setPosition (double incomingPosition)
{
    audioControl->setTransportPosition(incomingPosition);
    transport.setTransportPosition(incomingPosition);
}

void GuiControl::setVolume (double incomingVolume)
{
    audioControl->setVolume(incomingVolume);
    volumeControl.setVolume(incomingVolume);
}

//Text editor callbacks
void GuiControl::textEditorTextChanged (TextEditor &textEditor)
{
    musicTable->setFilterText(textEditor.getText());
    
    if (remoteConnections.getFirst() != nullptr)
        remoteConnections.getFirst()->sendTrackNums();
    
    int index = filteredDataList.indexOf(filteredDataList.getChildWithProperty("ID", tablePlayingRow.getProperty("ID")));
    if (index != -1)
        musicTable->getTableListBox().selectRow(index, false, true);
    
}


void GuiControl::showEffectsMenu()
{
    effectsTabbed = new TabbedComponent(TabbedButtonBar::TabsAtBottom);
    effectsTabbed->setSize(550, 300);
    
    equaliser = new Equaliser(audioControl);
    speedPitch = new SpeedPitch(audioControl);
    
    effectsTabbed->addTab("Equaliser", Colours::white, equaliser, false);
    effectsTabbed->addTab("Speed/Pitch", Colours::white, speedPitch, false);
    
    DialogWindow::showDialog("Effects", effectsTabbed, 0, Colours::white, true);
}

void GuiControl::setPlaylist (String incomingPlaylist)
{
    libraryView.setSelected(incomingPlaylist);
    changeListenerCallback(&libraryView);
}

void GuiControl::playingPlaylist(bool isPlayingPlaylist)
{
    if (isPlayingPlaylist)
    {
        currentlyPlayingList = filteredDataList.createCopy();
        currentlyPlayingName = libraryView.getSelectedPlaylist();
    }
    else
    {
        currentlyPlayingList = filteredDataList;
        currentlyPlayingName = libraryView.getSelectedPlaylist();
    }
}
