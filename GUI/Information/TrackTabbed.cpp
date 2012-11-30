//
//  TrackTabbed.cpp
//  MusicPlayer
//
//  Created by Andy on 29/11/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "TrackTabbed.h"

TrackTabbed::TrackTabbed (int selectedTrack) : TabbedComponent(TabbedButtonBar::TabsAtBottom)
{	   
    trackSummary.setTrack(selectedTrack);
    addTab("Summary", Colours::white, &trackSummary, true);
    
    setSize(trackSummary.getWidth(), trackSummary.getHeight());
}

TrackTabbed::~TrackTabbed()
{
	
}