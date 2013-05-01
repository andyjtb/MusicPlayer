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

/**
 The play button
 */
class PlayButton  : public Button,
                    public ValueTree::Listener
{
public:
    //==============================================================================
    /** Constructor */
    PlayButton ();
    /** Destructor */
    ~PlayButton();
	
	/** @internal */
    void resized();
    /** @internal */
    void paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown);
  
    //ValueTree Callbacks - Not used
    /**
     Required by ValueTree::Listener, not used
     */
    void valueTreePropertyChanged (ValueTree &treeWhosePropertyHasChanged, const Identifier &property){}
    /**
     Required by ValueTree::Listener, not used
     */
    void valueTreeChildAdded (ValueTree &parentTree, ValueTree &childWhichHasBeenAdded){}
    /**
     Required by ValueTree::Listener, not used
     */
    void valueTreeChildRemoved (ValueTree &parentTree, ValueTree &childWhichHasBeenRemoved){}
    /**
     Required by ValueTree::Listener, not used
     */
    void valueTreeChildOrderChanged (ValueTree &parentTreeWhoseChildrenHaveMoved){}
    /**
     Required by ValueTree::Listener, not used
     */
    void valueTreeParentChanged (ValueTree &treeWhoseParentHasChanged){}
    /** Makes sure that there is a file supposed to be being played before pressing the button has an effect.
     Stops the user from being able to toggle the play button before a song has been loaded
     */
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

