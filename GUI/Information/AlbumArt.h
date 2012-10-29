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

class AlbumArt  :	public ImageComponent
{
public:
    AlbumArt ();
    ~AlbumArt();
	
    void resized();
	void paint(Graphics& g);
	
	void setCover (Image cover);
	
private:
	void mouseDown(const MouseEvent &e);

	bool fileSelected;
	bool tagMissing; 
	
	Image newCover;
};


#endif   //H_ALBUMART