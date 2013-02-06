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
    
    libraryTreeView = new LibraryTreeView();
    addAndMakeVisible(libraryTreeView);
    
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
    
    libraryTreeView->setBounds((getWidth()/2)+50,100,100,100);
    
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
            int toPlay = filteredDataList.indexOf(tableSelectedRow);
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
		ITunesLibrary::getInstance()->saveLibrary();
		singletonLibraryTree = ITunesLibrary::getInstance()->getLibraryTree();
		ITunesLibrary::getInstance()->setLibraryTree(singletonLibraryTree);
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

