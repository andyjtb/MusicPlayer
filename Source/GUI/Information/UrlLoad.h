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

#include "JuceHeader.h"
/** A window which loads a picture from a URL and displays it before giving the user the option to accept this picture or not */
class UrlLoad  : public Component,
                 public Button::Listener
{
public:
    /** Constructor */
    UrlLoad ();
    /** Destructor */
    ~UrlLoad();

    /** @internal */
    void paint (Graphics& g) override;
    /** @internal */
    void resized() override;
    /** Loads the image from the url provided */
    void buttonClicked (Button* buttonThatWasClicked) override;
    /** Returns the loaded image*/
	Image& getImage();
    /** Whether the loaded file is a jped or a png */
	bool jpeg, png;

private:
    TextEditor urlEdit;
    TextButton urlLoad;
    ImageComponent preview;
	Image downloadedImage;

};


#endif //H_URLLOAD
