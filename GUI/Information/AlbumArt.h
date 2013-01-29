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

#include "../JuceLibraryCode/JuceHeader.h"
#include "UrlLoad.h"
#include "TagReader.h"
#include "Settings.h"

class AlbumArt  :	public ImageComponent
{
public:
    AlbumArt ();
    ~AlbumArt();
	
    void resized();
	void paint(Graphics& g);
	
	void setCover (File& incomingAudioFile);
    void setCover (Image cover);
    
    Image getCover();
    
    void fromFile();
    void fromUrl();
    
    void changeSize (double incomingSize);
    
private:
	void mouseDown(const MouseEvent &e);
    void mouseDoubleClick (const MouseEvent &e);
    void createPopup();
    
    int width, height;
	bool fileSelected, tagMissing, multiTrack;
    
    //Used to avoid copying around of rescaled image
	Image currentCover;
    File audioFile;
    
    int imageWidth, imageHeight;
};


#endif   //H_ALBUMART