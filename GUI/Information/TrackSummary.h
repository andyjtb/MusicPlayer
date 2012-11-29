/*
 *  TrackSummary.h
 *  MusicPlayer
 *
 *  Created by Andy on 29/11/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef TRACKSUMMARY
#define TRACKSUMMARY

#include "../JuceLibraryCode/JuceHeader.h"
#include "AlbumArt.h"
#include "TagReader.h"
#include "Settings.h"

class TrackSummary  : public Component
{
public:
    TrackSummary (int incomingTree);
    ~TrackSummary();

    void paint (Graphics& g);
    void resized();


private:

    Label artist;
    AlbumArt albumArt;
    Label album;
    Label song;
    Label locationLabel;
    Label location;
    Label typeLabel;
    Label type;
    Label sizeLabel;
    Label size;
    Label addedLabel;
    Label added;
    Label modifiedLabel;
    Label modified;
    Label bitLabel;
    Label bitRate;
    Label sampleLabel;
    Label sampleRate;

};


#endif   //TRACKSUMMARY
