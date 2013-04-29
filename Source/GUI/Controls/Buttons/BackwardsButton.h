/*
 *  BackwardsButton.h
 *  sdaMidiMeter
 *
 *  Created by Andy on 06/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef H_BACKWARDSBUTTON
#define H_BACKWARDSBUTTON

#include "../JuceLibraryCode/JuceHeader.h"

class BackwardsButton  :    public Button
{
public:
    BackwardsButton ();
    ~BackwardsButton();


    void resized();
    void paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown);
    void clicked();
    void buttonStateChanged();
	
    // Binary resources:
    static const char* backwardsDown_png;
    static const int backwardsDown_pngSize;
    static const char* backwards_png;
    static const int backwards_pngSize;

	String buttonName;
private:

    Image cachedImage_backwards_png;
    Image cachedImage_backwardsDown_png;

};


#endif   //H_BACKWARDSBUTTON
