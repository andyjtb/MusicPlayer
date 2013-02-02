//
//  TrackDialog.h
//  MusicPlayer
//
//  Created by Andy on 06/12/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef TRACKDIALOG 
#define TRACKDIALOG

#include "../JuceLibraryCode/JuceHeader.h"
#include "Settings.h"
#include "MusicLibraryHelpers.h"
#include "TrackTabbed.h"

class TrackDialog  :	public Component,
                    public ButtonListener
{
public:
    TrackDialog (int incomingTrack);
    ~TrackDialog();
	
    void paint (Graphics& g);
    void resized();
    
    void buttonClicked (Button* buttonClicked);
    
    void setTrack();
private:
    TrackTabbed trackTabbed;
    
    TextButton ok;
    TextButton next; 
    TextButton previous;
    
    //ValueTree filteredDataList;
    int selectedTrack;
};


#endif
