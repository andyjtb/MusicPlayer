//
//  TrackTabbed.h
//  MusicPlayer
//
//  Created by Andy on 29/11/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef TRACKTABBED
#define TRACKTABBED

#include "../JuceLibraryCode/JuceHeader.h"
#include "TrackSummary.h"
#include "TrackEdit.h"
#include "TrackArt.h"



class TrackTabbed  : public TabbedComponent
{
public:
    TrackTabbed ();
    ~TrackTabbed();
	
    void setTrack (ValueTree selectedTrack);
    
    void saveEdits();
    
private:
    TrackSummary trackSummary;
    TrackEdit trackEdit; 
    TrackArt trackArt;
};


#endif //TRACKTABBED
