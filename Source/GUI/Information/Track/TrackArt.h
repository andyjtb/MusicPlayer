//
//  TrackArt.h
//  MusicPlayer
//
//  Created by Andy on 09/12/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef TRACKART
#define TRACKART

#include "JuceHeader.h"
#include "MetaData/TagReader.h"
#include "Information/AlbumArt.h"
#include "Settings/Settings.h"

/** The album art for the track info display menu - Allows the image to be zoomed */
class TrackArt  :	public Component,
                    public Button::Listener,
public Slider::Listener
{
public:
    /** Constructor */
    TrackArt ();
    /** Destructor */
    ~TrackArt();
	
    /** @internal*/
    void paint (Graphics& g) override;
    /**@internal*/
    void resized() override;
    /** Sets the album art */
    void setTrack (ValueTree incomingTrack);

    /** Allows new art to be loaded either from a file or a URL */
    void buttonClicked (Button* buttonThatWasClicked) override;
    /** Zooms the image */
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    
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
