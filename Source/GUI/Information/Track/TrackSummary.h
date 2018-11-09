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

#include "JuceHeader.h"
#include "Information/AlbumArt.h"
#include "MetaData/TagReader.h"
#include "Settings/Settings.h"
#include "iTunesLibrary/MusicLibraryHelpers.h"

/** Displays a summary of all of the information contained about the selected file */
class TrackSummary  : public Component
{
public:
    /** Constructor */
    TrackSummary ();
    /** Destructor */
    ~TrackSummary();
    
    /** @internal */
    void paint (Graphics& g) override;
    /** @internal */
    void resized() override;
    
    /** Loads the information contained in the incomingTrack ValueTree, loads the album art for the file the location property refers to */
    void setTrack(ValueTree incomingTrack);
    
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
