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


class TrackTabbed  :	public TabbedComponent
{
public:
    TrackTabbed (int selectedTrack);
    ~TrackTabbed();

	
private:
    //ScopedPointer<TrackSummary> trackSummary;
    TrackSummary trackSummary;
    TrackEdit trackEdit;
};


#endif //TRACKTABBED
