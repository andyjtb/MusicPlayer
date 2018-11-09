/*
 *  AlbumArt.h
 *  MusicPlayer
 *
 *  Created by Andy on 29/10/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ALBUMART
#define ALBUMART

#include "JuceHeader.h"
#include "UrlLoad.h"
#include "MetaData/TagReader.h"
#include "Settings/Settings.h"

/** The album art display class used throughout the application */
class AlbumArt  :	public ImageComponent
{
public:
    /** Constructor */
    AlbumArt ();
    /** Destructor */
    ~AlbumArt();
	
    /** @internal */
    void resized() override;
    /** Draws the image on the component, if no album art is found displays a message saying so */
	void paint(Graphics& g) override;
	/** Sets the image only, does not save the incoming image to the files metadata */
    void setImageOnly (ImageWithType cover);
    /** Loads the image, Sets the image, saves the incoming image to the files metadata */
	void setCover (File& incomingAudioFile);
    /** Sets the image, saves the incoming image to the files metadata */
    void setCover (ImageWithType cover);
    /** Sets the image, saves the incoming image to the files metadata */
    void setCover (Image cover);
    
    /** Returns the image being displayed */
    Image getCover();
    
    /** Allows new album art to be loaded from a file and saved to the metadata tag */
    void fromFile();
    /** Allows new album art to be loaded from a URL and saved to the metadat tag */
    void fromUrl();
    /** Changes the size of the image, for the zoom function provided in TrackArt */
    void changeSize (double incomingSize);
    /** The currently displayed pictures file type */
    String extension;
    
private:
	void mouseDown(const MouseEvent &e) override;
    void mouseDoubleClick (const MouseEvent &e) override;
    void createPopup();
    
    int width, height;
	bool fileSelected, tagMissing, multiTrack;
    
    //Used to avoid copying around of rescaled image
	Image currentCover;
    File audioFile;
    
    int imageWidth, imageHeight;
};


#endif   //H_ALBUMART
