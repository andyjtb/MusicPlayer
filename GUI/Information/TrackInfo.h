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
#include "TagReader.h"
#include "MusicLibraryHelpers.h"
#include "Settings.h"

class TrackInfo  : public Component
{
public:
    TrackInfo ();
    ~TrackInfo();

    void resized();
	void loadTrackInfo(File& audioFile);

private:
    Label lTitle;
    Label lArtist;
    Label lId3;
    Label artist;
    Label title;
    Label id3;
    ToggleButton toggleId3;

};


#endif   //H_TRACKINFO
