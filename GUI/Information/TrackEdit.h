//
//  TrackEdit.h
//  MusicPlayer
//
//  Created by Andy on 03/12/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef TRACKEDIT
#define TRACKEDIT

#include "../JuceLibraryCode/JuceHeader.h"
#include "Settings.h"
#include "MusicLibraryHelpers.h"

class TrackEdit  :	public Component,
                    public SliderListener,
                    public ButtonListener
{
public:
    TrackEdit ();
    ~TrackEdit();
	
    void resized();
	void paint(Graphics& g);
	
    void setTrack(int incomingTrack);
	
    void sliderValueChanged (Slider* slider);
    void buttonClicked (Button* buttonClicked);
    
private:
    Label songLabel;
    TextEditor song;
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
    Label trackLabel;
    TextEditor trackNum;
    TextButton save;
};


#endif
