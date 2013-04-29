/*
 *  TrackInfo.h
 *  sdaMidiMeter
 *
 *  Created by Andy on 06/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef TRACKINFO
#define TRACKINFO

#include "../JuceLibraryCode/JuceHeader.h"
#include "MusicLibraryHelpers.h"
#include "Settings.h"

class TrackInfo  : public Component
{
public:
    TrackInfo ();
    ~TrackInfo();

    void resized();
	void loadTrackInfo(ValueTree tags);

private:
    Label artist;
    Label title;
    Label album;

};


#endif   //H_TRACKINFO
