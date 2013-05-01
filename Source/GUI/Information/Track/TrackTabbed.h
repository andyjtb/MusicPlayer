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

/** The tabbed component displayed when the user right clicks an item in the MusicLibraryTable and selects Display Info */
class TrackTabbed  : public TabbedComponent
{
public:
    /** Constructor */
    TrackTabbed ();
    /** Destructor */
    ~TrackTabbed();
	
    /** Loads all of the contained displays*/
    void setTrack (ValueTree selectedTrack);
    
    /** Saves any file information edits made */
    void saveEdits();
    
private:
    TrackSummary trackSummary;
    TrackEdit trackEdit; 
    TrackArt trackArt;
};


#endif //TRACKTABBED
