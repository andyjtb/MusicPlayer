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

class AlbumArt  :	public ImageComponent
{
public:
    AlbumArt ();
    ~AlbumArt();
	
    void resized();
	void paint(Graphics& g);
	
	void setCover (Image cover);
    
    void fromFile();
    void fromUrl();
    
    void changeSize (double incomingSize);
    
private:
	void mouseDown(const MouseEvent &e);
    
    int width, height;
	bool fileSelected;
	bool tagMissing; 
	
	Image newCover;
    
    int imageWidth, imageHeight;
};


#endif   //H_ALBUMART