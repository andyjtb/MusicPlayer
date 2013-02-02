//
//  TrackMulti.h
//  MusicPlayer
//
//  Created by Andy on 03/12/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef TRACKMULTI 
#define TRACKMULTI

#include "../JuceLibraryCode/JuceHeader.h"
#include "Settings.h"
#include "MusicLibraryHelpers.h"
#include "TagReader.h"
#include "AlbumArt.h"

class TrackMulti  :	public Component
{
public:
    TrackMulti (Array<int> incomingIds);
    ~TrackMulti();
	
    void resized();
	void paint(Graphics& g);

    void setInfo();
    
    void saveEdits();
    
private:
    Label artistLabel;
    TextEditor artist;
    Label albumLabel;
    TextEditor album;
    Label genreLabel;
    TextEditor genre;
    Label ratingLabel;
    Slider rating;
    Label labelLabel;
    TextEditor label;
    
    Label artLabel;
    AlbumArt art;
    
    Array<int> libraryIds;
    
    bool saveRequired, settingInfo;

    Image newArt;
};


#endif
