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
	//width = height = 175;
    
    fileSelected = false;
	tagMissing = false;
    
    //setSize (width, height);
	
}

AlbumArt::~AlbumArt()
{
	
}

//==============================================================================

void AlbumArt::resized()
{
//    if(findParentComponentOfClass<Viewport>())
//    {
//        setBounds (0, 0, imageWidth, imageHeight);
//    }
//    else
//    {
//        setBounds(0,0, 175, 175);
//    }
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
}

void AlbumArt::setCover (Image cover)
{

    
	if (cover.isValid()) {
		fileSelected = true;
        tagMissing = false;
        
        imageWidth = cover.getWidth();
        imageHeight = cover.getHeight();

		setImage (cover);
        
        if(findParentComponentOfClass<Viewport>())
        {
            setSize(getParentWidth(), getParentHeight());
        }
        //resized();

        //setSize(getWidth(), getHeight());
        
	}
	else {
		tagMissing = true;
		repaint();
	}
	
	
}

void AlbumArt::mouseDown(const MouseEvent & e) {
	
	ModifierKeys modifier = ModifierKeys::getCurrentModifiersRealtime();
	if (modifier.isPopupMenu() && fileSelected == true) 
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
        newCover = ImageFileFormat::loadFrom(fc.getResult());
        setImage(newCover);
    }
}

void AlbumArt::fromUrl()
{
    int result=0;
    UrlLoad urlLoad;
    AlertWindow urlAlert("Choose an album cover to open...", "You picked: ", AlertWindow::NoIcon);
    
    urlAlert.addCustomComponent(&urlLoad);
    urlAlert.addButton("Cancel", 0);
    urlAlert.addButton("Ok", 1);
    result = urlAlert.runModalLoop();
    urlAlert.removeCustomComponent(0);
    
    if (result == 0) {
        std::cout<< "cancelled";
    }
    if (result==1) {
        std::cout << "Ok";
        newCover = urlLoad.getImage();
        setImage (newCover);
        //sendActionMessage("albumart");
    }
}
