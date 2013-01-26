/*
 *  AlbumArt.cpp
 *  MusicPlayer
 *
 *  Created by Andy on 29/10/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "AlbumArt.h"

AlbumArt::AlbumArt ()
{	
    fileSelected = false;
	tagMissing = false;
	
}

AlbumArt::~AlbumArt()
{
	
}

//==============================================================================

void AlbumArt::resized()
{
}

void AlbumArt::paint(Graphics& g)
{	
	if (fileSelected == true && tagMissing == false) {
		g.setOpacity (1.0f);
		g.drawImageWithin (getImage(), 0, 0, getWidth(), getHeight(), getImagePlacement(), false);
	}
	
	if (tagMissing) {
		g.fillAll (Colours::white);
		g.setColour (Colours::black);
		g.setFont (Font (14.0000f, Font::plain));
		g.drawText ("NO ALBUM ART PRESENT",
					0, 0, getWidth(), getHeight(),
					Justification::centred, true);
		fileSelected = true;
		//tagMissing = false;
	}
    
    if (tagMissing && multiTrack)
    {
        g.fillAll (Colours::white);
		g.setColour (Colours::black);
		g.setFont (Font (14.0000f, Font::plain));
		g.drawText ("DOESN'T MATCH",
					0, 0, getWidth(), getHeight(),
					Justification::centred, true);
		fileSelected = true;
    }
}

void AlbumArt::setCover (File& incomingAudioFile)
{
    audioFile = incomingAudioFile;
    
    Image cover = TagReader::getAlbumArt(audioFile);
    
	if (cover.isValid() && cover.getWidth() >= 2) {
		fileSelected = true;
        tagMissing = false;
        
        imageWidth = cover.getWidth();
        imageHeight = cover.getHeight();
        
        if(findParentComponentOfClass<Viewport>())
        {
            setSize(getParentWidth(), getParentHeight());
        }
        currentCover = cover;
        setImage (cover);
	}
	else {
		tagMissing = true;
        if (cover.getWidth() == 1)
        {
            multiTrack = true;
        }
		repaint();
	}
	
	
}

void AlbumArt::setCover (Image cover)
{
	if (cover.isValid() && cover.getWidth() >= 2) {
		fileSelected = true;
        tagMissing = false;
        
        imageWidth = cover.getWidth();
        imageHeight = cover.getHeight();
        
        if(findParentComponentOfClass<Viewport>())
        {
            setSize(getParentWidth(), getParentHeight());
        }
        currentCover = cover;
        setImage (cover);
	}
	else {
		tagMissing = true;
        if (cover.getWidth() == 1)
        {
            multiTrack = true;
        }
		repaint();
	}
	
	
}

Image AlbumArt::getCover()
{
    return currentCover;
}

void AlbumArt::mouseDown(const MouseEvent &e) {
	
	ModifierKeys modifier = ModifierKeys::getCurrentModifiersRealtime();
	if (modifier.isPopupMenu() && fileSelected == true) 
	{
        createPopup();
	}
}

void AlbumArt::mouseDoubleClick(const MouseEvent &e)
{
    createPopup();
}

void AlbumArt::createPopup()
{
    PopupMenu popupMenu;
    popupMenu.addItem(1,"Add from File",true);
    popupMenu.addItem(2,"Add from URL",true);
    
    int menuResult = popupMenu.show();
    
    if (menuResult==1) 
    {
        fromFile();
        
    }
    
    if (menuResult == 2) {
        fromUrl();
    }
}

void AlbumArt::changeSize(double incomingSize)
{
    if (!tagMissing)
    {
        imageWidth = imageHeight = incomingSize;
        setSize(incomingSize, incomingSize);
        
        if (incomingSize > imageHeight) {
            setImage(getImage().rescaled((getParentWidth() + (incomingSize/2)), (getParentHeight() + (incomingSize/2)), Graphics::highResamplingQuality));
        }
    }
}

void AlbumArt::fromFile()
{
    FileChooser fc ("Choose an album cover to open...",
                    File::getCurrentWorkingDirectory(),
                    "*.jpg;*.jpeg;*.png");
    
    ImagePreviewComponent imagePreview;
    imagePreview.setSize (200, 200);
    
    if (fc.browseForFileToOpen(&imagePreview))
    {				
        currentCover = ImageFileFormat::loadFrom(fc.getResult());
        setCover(currentCover);
        TagReader::saveAlbumArt(audioFile, currentCover);
    }
}

void AlbumArt::fromUrl()
{
    UrlLoad urlLoad;
    AlertWindow urlAlert("Choose an album cover to open...", "You picked: ", AlertWindow::NoIcon);
    
    urlAlert.addCustomComponent(&urlLoad);
    urlAlert.addButton("Cancel", 0);
    urlAlert.addButton("Ok", 1);
    
    if (urlAlert.runModalLoop() != 0) {
        currentCover = urlLoad.getImage();
        setCover(currentCover);
        TagReader::saveAlbumArt(audioFile, currentCover);
    }
    
    urlAlert.removeCustomComponent(0);
}
