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
    
    addAndMakeVisible(&infoBar);
	
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
	playButton.setBounds(0, 0, 143, 143);
	volumeControl.setBounds(0, 143, 380, 80);
	outputMeters.setBounds(0, 250, 250, 80);
	transport.setBounds(150, 20, 300, 60);
	trackInfo.setBounds(180, 100, 270, 150);
	albumArt.setBounds(400,100,175,175);
    
    infoBar.setBounds(0, getHeight()-15, getWidth(), 15);
    
	searchBox.setBounds(getWidth()-200, 0, 175, 60);

    musicLibraryDropTarget.setBounds(0, getHeight()/2, getWidth(), (getHeight()/2)-15);
    //musicTable.setBounds(0, getHeight()/2, getWidth(), getHeight()/2);

    libraryView.setBounds(600, 100, 200, 200);
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
            
            int toPlay = filteredDataList.indexOf(tablePlayingRow);
            if (toPlay != -1)
                toPlay++;
            
            ValueTree test(filteredDataList.getChild(toPlay));
            if (test.isValid()) {
                tableSelectedRow = test;
                loadingNew = true;
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
        DBG("Current lib id = " << singletonCurrentLibId);
        singletonCurrentValueTreeId = singletonLibraryTree.getChild(singletonLibraryTree.getNumChildren()-1).getProperty(MusicColumns::columnNames[MusicColumns::ID]);
        
        singletonCurrentPlaylistId = singletonPlaylistsTree.getChild(singletonPlaylistsTree.getNumChildren()-1).getProperty(MusicColumns::playlistID);
        
        DBG("Library file = " << singletonLibraryFile.getFullPathName());
        DBG("Playlist file = " << singletonPlaylistsFile.getFullPathName());
        
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
            musicTable->getTableListBox().selectRow(filteredDataList.indexOf(tablePlayingRow));
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
        //iTunes feature, when a new song is to be played it moves the table to that song
        if (!loadingNew)
            musicTable->getTableListBox().selectRow(filteredDataList.indexOf(tablePlayingRow));
        
        loadingNew = false;
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
        //iTunes feature, pausing or playing changes the view to the currently playing track
        musicTable->getTableListBox().selectRow(filteredDataList.indexOf(tablePlayingRow));
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
    if (musicTable->isTableDeleting() != true)
    {
        File selectedFile (tableSelectedRow.getProperty(MusicColumns::columnNames[MusicColumns::Location]));
        
        DBG("Gui load = " << selectedFile.getFileName());
        
        if (selectedFile.existsAsFile())
        {
            int result = 0;
            
            if(tableShouldPlay.getValue())
            {
                DBG("Load file " << selectedFile.getFullPathName());
                singletonPlayState = false;
                result = audioControl->loadFile(selectedFile);
                tablePlayingRow = tableSelectedRow;
                singletonPlayState = true;
                trackInfo.loadTrackInfo(tableSelectedRow);
                tableShouldPlay.setValue(false);
            }
            if (tableLoadSelected.getValue())
            {
                singletonPlayState = false;
                result = audioControl->loadFile(selectedFile);
                setPosition(0);
                tablePlayingRow = tableSelectedRow;
                trackInfo.loadTrackInfo(tableSelectedRow);
                tableLoadSelected = false;
            }
            
            infoBar.displayFileStatus(selectedFile, result);
            albumArt.setCover(selectedFile);
        }
        else
        {
            //File not found
            infoBar.displayFileStatus(selectedFile, 1);
        }
    }
    
}

void GuiControl::updateTagDisplay (File audioFile)
{
	albumArt.setCover(audioFile);
}

void GuiControl::next()
{
    if (tableSelectedRow.isValid()) {
        int toPlay = filteredDataList.indexOf(tableSelectedRow);
        ++toPlay;
        if (toPlay < filteredDataList.getNumChildren())
        {
            tableSelectedRow = filteredDataList.getChild(toPlay);
            if (singletonPlayState.getValue())
                tableShouldPlay = true;
            else
                tableLoadSelected = true;
        }
    }
}

void GuiControl::previous()
{
    if (tableSelectedRow.isValid()) {
        int toPlay = filteredDataList.indexOf(tableSelectedRow);
        --toPlay;
        if (toPlay >= 0)
        {
            tableSelectedRow = filteredDataList.getChild(toPlay);
            if (singletonPlayState.getValue())
                tableShouldPlay = true;
            else
                tableLoadSelected = true;
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
    
}
void GuiControl::textEditorReturnKeyPressed (TextEditor &textEditor)
{}
void GuiControl::textEditorEscapeKeyPressed (TextEditor &textEditor)
{}
void GuiControl::textEditorFocusLost (TextEditor &textEditor)
{}

//ValueTree Callbacks
void GuiControl::valueTreePropertyChanged (ValueTree &treeWhosePropertyHasChanged, const Identifier &property)
{
}
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
    //if (treeWhichHasBeenChanged.getProperty(MusicColumns::columnNames[MusicColumns::LibID]) != tableSelectedRow.getProperty(MusicColumns::columnNames[MusicColumns::LibID]))
    DBG("changed = " << treeWhichHasBeenChanged.getProperty(MusicColumns::columnNames[MusicColumns::LibID]).toString());
    DBG("singleton = " << tableSelectedRow.getProperty(MusicColumns::columnNames[MusicColumns::LibID]).toString());
    
    loadFile();

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

