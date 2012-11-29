/*
 *  GuiControl.h
 *  MusicPlayer
 *
 *  Created by Andy on 26/10/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef H_GUICONTROL
#define H_GUICONTROL

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioControl.h"
#include "PlayButton.h"
#include "BackwardsButton.h"
#include "ForwardButton.h"
#include "OutputMeter.h"
#include "VolumeControl.h"
#include "TransportSlider.h"
#include "TrackInfo.h"
#include "AlbumArt.h"
#include "MusicLibraryTableModel.h"
#include "MusicLibraryHelpers.h"
#include "Settings.h"
//#include "CoverFlowComponent.h"

/**

 */
class GuiControl  : public Component,
					public ButtonListener,
					public ActionListener,
					public ChangeListener,
					public ValueListener,
					public MultiTimer
{
public:
	//==============================================================================
    /**
     Constructor
     */
	GuiControl();
    
    /**
     Destructor
     */
	~GuiControl();
	void resized();
	
	void setAudioControl (AudioControl* incomingAudioControl);
	
	void buttonClicked (Button* buttonClicked);
	
	void actionListenerCallback (const String& message);
	
	void valueChanged (Value& valueChanged);

	void timerCallback(int timerId);
	
	void updateTagDisplay (File audioFile);
	
	void changeListenerCallback(ChangeBroadcaster* changeBroadcaster);
    
    void loadFile();
	
private:
	AudioControl* audioControl;
	
	PlayButton playButton;
	VolumeControl volumeControl;
	OutputMeter outputMeters;
	TransportSlider transport;
	TrackInfo trackInfo;
	AlbumArt albumArt;
    
    MusicLibraryTable musicTable;
    
//    CoverFlowComponent* coverflow;
};



#endif //H_GUICONTROL