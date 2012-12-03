/*
 *  PlayButton.h
 *  sdaMidiMeter
 *
 *  Created by Andy on 06/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef H_PLAYBUTTON
#define H_PLAYBUTTON

#include "../JuceLibraryCode/JuceHeader.h"
#include "Settings.h"

class PlayButton  : public Button,
					public ActionBroadcaster
{
public:
    //==============================================================================
    PlayButton ();
    ~PlayButton();
	
	
    void resized();
    void paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown);
  
    // Binary resources:
    static const char* pauseDown_png;
    static const int pauseDown_pngSize;
    static const char* pause_png;
    static const int pause_pngSize;
    static const char* play_png;
    static const int play_pngSize;
    static const char* playDown_png;
    static const int playDown_pngSize;
	
	
private:
	
    //==============================================================================
    Image cachedImage_play_png;
    Image cachedImage_playDown_png;
    Image cachedImage_pause_png;
    Image cachedImage_pauseDown_png;
	

    //==============================================================================

};


#endif //H_PLAYBUTTON

