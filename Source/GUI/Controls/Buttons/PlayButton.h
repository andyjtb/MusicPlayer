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
                    public ValueTree::Listener
{
public:
    //==============================================================================
    PlayButton ();
    ~PlayButton();
	
	
    void resized();
    void paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown);
  
    //ValueTree Callbacks
    void valueTreePropertyChanged (ValueTree &treeWhosePropertyHasChanged, const Identifier &property);
    void valueTreeChildAdded (ValueTree &parentTree, ValueTree &childWhichHasBeenAdded);
    void valueTreeChildRemoved (ValueTree &parentTree, ValueTree &childWhichHasBeenRemoved);
    void valueTreeChildOrderChanged (ValueTree &parentTreeWhoseChildrenHaveMoved);
    void valueTreeParentChanged (ValueTree &treeWhoseParentHasChanged);
    void valueTreeRedirected (ValueTree &treeWhichHasBeenChanged);
    
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

