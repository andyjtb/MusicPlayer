/*
 *  GuiControl.cpp
 *  MusicPlayer
 *
 *  Created by Andy on 26/10/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "GuiControl.h"


GuiControl::GuiControl()
{	
	addAndMakeVisible(&playButton);
	playButton.addListener(this);
	
	addAndMakeVisible(&volumeControl);
	volumeControl.addValueListener(this);
	
	addAndMakeVisible(&outputMeters);
	
	addAndMakeVisible(&transport);
	transport.addActionListener(this);
	
	addAndMakeVisible(&trackInfo);
	
	addAndMakeVisible(&albumArt);
	
	ITunesLibrary::getInstance()->setLibraryTree (singletonLibraryTree);
    
	musicTable.setLibraryToUse (ITunesLibrary::getInstance());
	musicTable.addActionListener(this);
    tableSelectedRow.addListener(this);
    tableShouldPlay.addListener(this);
	addAndMakeVisible(&musicTable);
    
	//setSize(500, 400);
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
	
    musicTable.setBounds(0, getHeight()/2, getWidth(), getHeight()/2);
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
	}
}

void GuiControl::setAudioControl(AudioControl* incomingAudioControl)
{
    audioControl = incomingAudioControl;
	audioControl->addChangeListener(this);
	
//	playButton.getButtonValue().addListener(audioControl);
} 

void GuiControl::buttonClicked (Button* buttonClicked)
{
	if (buttonClicked == &playButton) {
		if (audioControl->isPlaying()) {
			audioControl->setPlayState(false);
			//stopTimer(0);
			stopTimer(1);
		}
		else {
			audioControl->setPlayState(true);
			startTimer(0, 50);
			startTimer(1, 100);
		}
	}
}

void GuiControl::actionListenerCallback (const String& message)
{
//	if (message.startsWith("transportLength")) 
//    {
//        String subString = message.fromFirstOccurrenceOf (":", false, true);
//        double value = subString.getDoubleValue();
//		if (value < 1) {
//			transport.setTransportRange(0, value, 0.01);
//		}
//		else {
//			transport.setTransportRange(0, value, 0.1);
//		}
//		
//    }
//	
    if (message == "LibraryImportFinished") {
		DBG("library Loaded");
		ITunesLibrary::getInstance()->saveLibrary(singletonLibraryFile);
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
	double value = audioControl->getTransportLength();
	
	if (changeBroadcaster == audioControl) {
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
	}
    
    if (valueChanged == tableSelectedRow)
    {
        loadFile();        
    }
    
    if (valueChanged == tableShouldPlay)
    {
        loadFile();
    }
}

void GuiControl::loadFile()
{
    File selectedFile (singletonLibraryTree.getChild(tableSelectedRow.getValue()).getProperty(MusicColumns::columnNames[MusicColumns::Location]));
    if(tableShouldPlay.getValue())
    {
        audioControl->loadFile(selectedFile);
        buttonClicked(&playButton);
        trackInfo.loadTrackInfo(tableSelectedRow.getValue());
        tableShouldPlay.setValue(false);
    }
    
    albumArt.setCover(TagReader::getAlbumArt(selectedFile));
}

void GuiControl::updateTagDisplay (File audioFile)
{
//	trackInfo.loadTrackInfo(audioFile);
	albumArt.setCover(TagReader::getAlbumArt(audioFile));
}