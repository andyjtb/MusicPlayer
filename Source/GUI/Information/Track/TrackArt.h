//
//  TrackArt.h
//  MusicPlayer
//
//  Created by Andy on 09/12/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef TRACKART
#define TRACKART

#include "../JuceLibraryCode/JuceHeader.h"
#include "TagReader.h"
#include "AlbumArt.h"
#include "Settings.h"

/** The album art for the track info display menu - Allows the image to be zoomed */
class TrackArt  :	public Component,
                    public ButtonListener,
public Slider::Listener
{
public:
    /** Constructor */
    TrackArt ();
    /** Destructor */
    ~TrackArt();
	
    /** @internal*/
    void paint (Graphics& g);
    /**@internal*/
    void resized();
    /** Sets the album art */
    void setTrack (ValueTree incomingTrack);

    /** Allows new art to be loaded either from a file or a URL */
    void buttonClicked (Button* buttonThatWasClicked);
    /** Zooms the image */
    void sliderValueChanged (Slider* sliderThatWasMoved);
    
    /** Saves the newly loaded art*/
    void saveArt();
    
private:
    Viewport view;
    AlbumArt albumArt;
    Label sizeLabel;
    Slider size;
    TextButton file;
    TextButton url;

    File selectedFile;
    
    ValueTree selectedTrack;
    
    ImageWithType cover;
};



#endif
