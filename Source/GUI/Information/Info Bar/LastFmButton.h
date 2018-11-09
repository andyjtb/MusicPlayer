/*
 *  LastFmButton.h
 *  MusicPlayer
 *
 *  Created by Andy on 26/10/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef LASTFMBUTTON
#define LASTFMBUTTON

#include "JuceHeader.h"
#include "Settings/Settings.h"

//==============================================================================
/**
Last.fm icon
*/
class LastFmButton  : public Component
{
public:
    LastFmButton ();
    ~LastFmButton();

    void paint (Graphics& g) override;

    void mouseDown(const MouseEvent& e) override;
    
    void setConnected (bool isConnected);
    
    void displayError (int error, String message);
    
    static const char* rectBlack_png;
    static const int rectBlack_pngSize;
    static const char* rectRed_png;
    static const int rectRed_pngSize;

private:
    Image cachedImage_rectBlack_png;
    Image cachedImage_rectRed_png;
};

#endif
