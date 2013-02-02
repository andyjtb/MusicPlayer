/*
 *  UrlLoad.h
 *  sdaMidiMeter
 *
 *  Created by Andy on 10/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef H_URLLOAD
#define H_URLLOAD

#include "../JuceLibraryCode/JuceHeader.h"

class UrlLoad  : public Component,
                 public ButtonListener
{
public:
    UrlLoad ();
    ~UrlLoad();


    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
	Image& getImage();
	bool jpeg, png;

private:
    TextEditor urlEdit;
    TextButton urlLoad;
    ImageComponent preview;
	Image downloadedImage;

};


#endif //H_URLLOAD
