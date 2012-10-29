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
	
	setSize(500, 400);
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
	audioControl->addActionListener(this);
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
	if (message.startsWith("transportLength")) 
    {
        String subString = message.fromFirstOccurrenceOf (":", false, true);
        double value = subString.getDoubleValue();
		if (value < 1) {
			transport.setTransportRange(0, value, 0.01);
		}
		else {
			transport.setTransportRange(0, value, 0.1);
		}
		
    }
	
	if (message.startsWith("transportPosition")) 
    {
        String subString = message.fromFirstOccurrenceOf (":", false, true);
        double value = subString.getDoubleValue();
		
		audioControl->setTransportPosition(value);
    }
}

void GuiControl::valueChanged (Value& valueChanged)
{
	if (valueChanged == volumeControl.getSliderValue()) {
		audioControl->setVolume(valueChanged.getValue());
	}
}

void GuiControl::updateTagDisplay (File audioFile)
{
	trackInfo.loadTrackInfo(audioFile);
	albumArt.setCover(TagReader::getAlbumArt(audioFile));
}