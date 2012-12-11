/*
 *  TagReader.cpp
 *  sdaMidiMeter
 *
 *  Created by Andy on 10/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "UrlLoad.h"

UrlLoad::UrlLoad ()
    : tUrl (0),
      urlLoad (0),
      preview (0)
{
    addAndMakeVisible (tUrl = new TextEditor (String::empty));
    tUrl->setMultiLine (false);
    tUrl->setReturnKeyStartsNewLine (false);
    tUrl->setReadOnly (false);
    tUrl->setScrollbarsShown (true);
    tUrl->setCaretVisible (true);
    tUrl->setPopupMenuEnabled (true);
    tUrl->setText (String::empty);

    addAndMakeVisible (urlLoad = new TextButton (String::empty));
    urlLoad->setButtonText ("Load");
    urlLoad->addListener (this);

    addAndMakeVisible (preview = new ImageButton (String::empty));
    preview->setButtonText ("new button");

    preview->setImages (false, true, true,
                            Image(), 1.0000f, Colour (0x0),
                            Image(), 1.0000f, Colour (0x0),
                            Image(), 1.0000f, Colour (0x0));


    setSize (400, 400);

}

UrlLoad::~UrlLoad()
{

    deleteAndZero (tUrl);
    deleteAndZero (urlLoad);
    deleteAndZero (preview);

}

//==============================================================================
void UrlLoad::paint (Graphics& g)
{
    g.fillAll (Colour (0xffffff));

}

void UrlLoad::resized()
{
    tUrl->setBounds (0, 0, 400, 30);
    urlLoad->setBounds (152, 32, 100, 24);
    preview->setBounds (40, 72, 336, 280);
}

void UrlLoad::buttonClicked (Button* buttonThatWasClicked)
{

    if (buttonThatWasClicked == urlLoad)
    {
        URL imageURL = tUrl->getText();
		
		if (imageURL.isProbablyAWebsiteURL(tUrl->getText())) {
			bool jpeg;
			bool png;
			
			if (imageURL.toString(false).endsWith("jpeg") || imageURL.toString(false).endsWith("jpg"))
			{ jpeg = true;}
			if (imageURL.toString(false).endsWith("png")) {
				png = true;
			}
			
			if (!jpeg && !png) {
				AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
												  "URL Invalid",
												  "Please re-enter URL");
			}
			
			if (jpeg || png) {
				MemoryBlock imageData;
				imageURL.readEntireBinaryStream(imageData, false);
			
				downloadedImage = ImageFileFormat::loadFrom(imageData.getData(), imageData.getSize());
				
				preview->setImages (false, true, true,
									downloadedImage, 1.0000f, Colour (0x0),
									downloadedImage, 1.0000f, Colour (0x0),
									downloadedImage, 1.0000f, Colour (0x0));
			}
			
		}
		else {
			AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
											  "URL Invalid",
											  "Please re-enter URL");
		}

    }

}

Image& UrlLoad::getImage()
{
	return downloadedImage;
}