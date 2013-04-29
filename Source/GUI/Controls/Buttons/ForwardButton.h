/*
 *  ForwardButton.h
 *  sdaMidiMeter
 *
 *  Created by Andy on 06/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef H_FORWARDBUTTON
#define H_FORWARDBUTTON


#include "../JuceLibraryCode/JuceHeader.h"

class ForwardButton  :  public Button
{
public:
    ForwardButton ();
    ~ForwardButton();

    void resized();
    void paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown);
    void clicked();
    void buttonStateChanged();
	
    // Binary resources:
    static const char* forwardDown_png;
    static const int forwardDown_pngSize;
    static const char* forward_png;
    static const int forward_pngSize;

	
private:

    Image cachedImage_forward_png;
    Image cachedImage_forwardDown_png;

};


#endif //H_FORWARDBUTTON
