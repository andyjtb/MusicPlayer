//
//  TrackTabbed.cpp
//  MusicPlayer
//
//  Created by Andy on 29/11/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "TrackTabbed.h"

TrackTabbed::TrackTabbed () : TabbedComponent(TabbedButtonBar::TabsAtBottom)
{	   
    addTab("Summary", Colours::white, &trackSummary, true);
    addTab("Edit Info", Colours::white, &trackEdit, true);
    addTab("Album Art", Colours::white, &trackArt, true);
    
    setSize(trackSummary.getWidth(), trackSummary.getHeight());
}

TrackTabbed::~TrackTabbed()
{
	
}

void TrackTabbed::setTrack (ValueTree selectedTrack)
{
    trackSummary.setTrack(selectedTrack);
    trackEdit.setTrack(selectedTrack);
    //trackEdit.setSaveRequired(false);
    trackArt.setTrack(selectedTrack);
}

void TrackTabbed::saveEdits()
{
    trackEdit.saveEdits();
}