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

GuiControl::GuiControl()
{	
	addAndMakeVisible(&playButton);
	//playButton.addListener(this);
	
	addAndMakeVisible(&volumeControl);
	volumeControl.addValueListener(this);
	
	addAndMakeVisible(&outputMeters);
	
	addAndMakeVisible(&transport);
	transport.addActionListener(this);
	
	addAndMakeVisible(&trackInfo);
	
	addAndMakeVisible(&albumArt);
	
    addAndMakeVisible(&searchBox);
    searchBox.getSearchTextEditor().addListener(this);
    
	ITunesLibrary::getInstance()->setLibraryTree (singletonLibraryTree);
    ITunesLibrary::getInstance()->setPlaylistsTree(singletonPlaylistsTree);
    musicTable = musicLibraryDropTarget.getMusicTable();
	musicTable->setLibraryToUse (ITunesLibrary::getInstance());
	musicTable->addActionListener(this);
    tableSelectedRow.addListener(this);
    tableShouldPlay.addListener(this);
    tableLoadSelected.addListener(this);
    tableUpdateRequired.addListener(this);
	//addAndMakeVisible(&musicTable);
    
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

void GuiControl::resized()
{
	playButton.setBounds(0, 0, 143, 143);
	volumeControl.setBounds(0, 143, 380, 80);
	outputMeters.setBounds(0, 250, 250, 80);
	transport.setBounds(150, 20, 300, 60);
	trackInfo.setBounds(180, 100, 270, 150);
	albumArt.setBounds(400,100,175,175);
	searchBox.setBounds(getWidth()-200, 0, 175, 60);

    musicLibraryDropTarget.setBounds(0, getHeight()/2, getWidth(), getHeight()/2);
    //musicTable.setBounds(0, getHeight()/2, getWidth(), getHeight()/2);

    libraryView.setBounds(600, 100, 200, 200);
    //libraryTreeView.setBounds((getWidth()/2)+50,100,100,100);
    //treeViewDemo->setBounds((getWidth()/2)+100,50,400,200);
//    coverflow->setBounds(100,100,100,100);
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
            
            int toPlay = filteredDataList.indexOf(tablePlayingRow);
            toPlay++;
            
            ValueTree test(filteredDataList.getChild(toPlay));
            if (test.isValid()) {
                tableSelectedRow = test;
                tableShouldPlay = true;
            }
            else
            {
                singletonPlayState = false;
            }
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
        
        singletonCurrentValueTreeId = singletonLibraryTree.getChild(singletonLibraryTree.getNumChildren()-1).getProperty(MusicColumns::columnNames[MusicColumns::ID]);
        
        singletonCurrentPlaylistId = singletonPlaylistsTree.getChild(singletonPlaylistsTree.getNumChildren()-1).getProperty(MusicColumns::playlistID);
        
        DBG("Library file = " << singletonLibraryFile.getFullPathName());
        DBG("Playlist file = " << singletonPlaylistsFile.getFullPathName());
        
		ITunesLibrary::getInstance()->saveLibrary();
		singletonLibraryTree = ITunesLibrary::getInstance()->getLibraryTree();
		ITunesLibrary::getInstance()->setLibraryTree(singletonLibraryTree);
        
        musicTable->setFilterText(String::empty);
	}
    
	if (message.startsWith("transportPosition")) 
    {
        String subString = message.fromFirstOccurrenceOf (":", false, true);
        double value = subString.getDoubleValue();
		
		audioControl->setTransportPosition(value);
    }
}

void GuiControl::changeListenerCallback (ChangeBroadcaster* changeBroadcaster)
{
    //New song loaded change message
	if (changeBroadcaster == audioControl) {
        double value = audioControl->getTransportLength();
        audioControl->setVolume(volumeControl.getVolume());
        if(remoteConnections.getFirst() != nullptr)
        {
            remoteConnections.getFirst()->sendPlayingData();
        }
        
		if (value < 1) {
			transport.setTransportRange(0, value, 0.01);
		}
		else {
			transport.setTransportRange(0, value, 0.1);
		}
	}
    
    if (changeBroadcaster == &libraryView)
    {
        if (libraryView.getSelectedPlaylist() == "Library")
        {
            musicTable->changeDisplay(false);
            musicTable->setSortColumn(MusicColumns::Artist);
        }
        else
        {
            //Saves current tree to desktop for debugging
            File test = File(File::getSpecialLocation(File::userDesktopDirectory).getChildFile("PlaylistTest.xml"));
            writeValueTreeToFile(singletonPlaylistsTree, test);
            
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
                        DBG("Loading id " << loadID);
                    }
                }
                
                playlistValueTree.addChild(toLoad, -1, 0);
                
                musicTable->setPlaylistTree (playlistValueTree);
                musicTable->changeDisplay(true);
                musicTable->setSortColumn(MusicColumns::LibID);
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
    
    if (valueChanged == tableShouldPlay)
    {
        loadFile();
    }
    
    if (valueChanged == tableLoadSelected)
    {
        loadFile();
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
	}
    
    if (valueChanged == artUpdateRequired)
    {
        if(artUpdateRequired.getValue())
        {
            File audioFile = tableSelectedRow.getProperty("Location").toString();
            ImageWithType currentCover = TagReader::getAlbumArt(audioFile);
            albumArt.setCover(currentCover);
            artUpdateRequired = false;
        }
    }
}
void GuiControl::loadFile()
{
    if (tableDeleting != true)
    {
        File selectedFile (tableSelectedRow.getProperty(MusicColumns::columnNames[MusicColumns::Location]));
        if(tableShouldPlay.getValue())
        {
            singletonPlayState = false;
            audioControl->loadFile(selectedFile);
            tablePlayingRow = tableSelectedRow;
            singletonPlayState = true;
            trackInfo.loadTrackInfo(tableSelectedRow);
            tableShouldPlay.setValue(false);
        }
        if (tableLoadSelected.getValue())
        {
            singletonPlayState = false;
            audioControl->loadFile(selectedFile);
            setPosition(0);
            tablePlayingRow = tableSelectedRow;
            trackInfo.loadTrackInfo(tableSelectedRow);
            tableLoadSelected = false;
        }
        
        albumArt.setCover(selectedFile);
    }
}

void GuiControl::updateTagDisplay (File audioFile)
{
	albumArt.setCover(audioFile);
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
    
}
void GuiControl::textEditorReturnKeyPressed (TextEditor &textEditor)
{}
void GuiControl::textEditorEscapeKeyPressed (TextEditor &textEditor)
{}
void GuiControl::textEditorFocusLost (TextEditor &textEditor)
{}

//ValueTree Callbacks
void GuiControl::valueTreePropertyChanged (ValueTree &treeWhosePropertyHasChanged, const Identifier &property)
{}
void GuiControl::valueTreeChildAdded (ValueTree &parentTree, ValueTree &childWhichHasBeenAdded)
{}
void GuiControl::valueTreeChildRemoved (ValueTree &parentTree, ValueTree &childWhichHasBeenRemoved)
{}
void GuiControl::valueTreeChildOrderChanged (ValueTree &parentTreeWhoseChildrenHaveMoved)
{}
void GuiControl::valueTreeParentChanged (ValueTree &treeWhoseParentHasChanged)
{}
void GuiControl::valueTreeRedirected (ValueTree &treeWhichHasBeenChanged)
{  
    //Selected Row change listener
    loadFile();
}

