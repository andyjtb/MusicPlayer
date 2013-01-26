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
{
    addAndMakeVisible (&urlEdit);
    urlEdit.setMultiLine (false);
    urlEdit.setReturnKeyStartsNewLine (false);
    urlEdit.setReadOnly (false);
    urlEdit.setScrollbarsShown (true);
    urlEdit.setCaretVisible (true);
    urlEdit.setPopupMenuEnabled (true);
    urlEdit.setText (String::empty);

    addAndMakeVisible (&urlLoad);
    urlLoad.setButtonText ("Load");
    urlLoad.addListener (this);

    addAndMakeVisible (&preview);


    setSize (400, 400);

}

UrlLoad::~UrlLoad()
{
}

//==============================================================================
void UrlLoad::paint (Graphics& g)
{
    g.fillAll (Colour (0xffffff));

}

void UrlLoad::resized()
{
    urlEdit.setBounds (0, 0, 400, 30);
    urlLoad.setBounds (152, 32, 100, 24);
    preview.setBounds (40, 72, 336, 280);
}

void UrlLoad::buttonClicked (Button* buttonThatWasClicked)
{

    if (buttonThatWasClicked == &urlLoad)
    {
        URL imageURL = urlEdit.getText();
		
		if (imageURL.isProbablyAWebsiteURL(urlEdit.getText())) {
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
				
				preview.setImage(downloadedImage);
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