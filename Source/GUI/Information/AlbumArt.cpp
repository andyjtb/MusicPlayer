/*
 *  AlbumArt.cpp
 *  MusicPlayer
 *
 *  Created by Andy on 29/10/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "AlbumArt.h"
#include "GuiControl.h"

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
	
	if (tagMissing && !multiTrack) {
		g.fillAll (Colours::white);
		g.setColour (Colours::black);
		g.setFont (Font (14.0000f, Font::plain));
		g.drawText ("NO ALBUM ART PRESENT",
					0, 0, getWidth(), getHeight(),
					Justification::centred, true);
		fileSelected = true;
		tagMissing = false;
        multiTrack = false;
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
        tagMissing = false;
        multiTrack = false;
    }
}

void AlbumArt::setImageOnly (ImageWithType cover)
{
    if (cover.image.isValid() && cover.image.getWidth() > 2) {
		fileSelected = true;
        tagMissing = false;
        
        imageWidth = cover.image.getWidth();
        imageHeight = cover.image.getHeight();
        
        if(findParentComponentOfClass<Viewport>())
        {
            setSize(getParentWidth(), getParentHeight());
        }
        
        if (audioFile.existsAsFile())
        {
            currentCover = cover.image;
            extension = cover.type;
            
            setImage (cover.image);
        }
	}
	else {
		tagMissing = true;
        if (cover.image.getWidth() == 2)
        {
            multiTrack = true;
        }
		repaint();
	}
}

void AlbumArt::setCover (File& incomingAudioFile)
{
    audioFile = incomingAudioFile;
    
    ImageWithType cover = TagReader::getAlbumArt(audioFile);
    
	if (cover.image.isValid() && cover.image.getWidth() > 2) {
		fileSelected = true;
        tagMissing = false;
        
        imageWidth = cover.image.getWidth();
        imageHeight = cover.image.getHeight();
        
        if(findParentComponentOfClass<Viewport>())
        {
            setSize(getParentWidth(), getParentHeight());
        }
        currentCover = cover.image;
        extension = cover.type;
        setImage (cover.image);
	}
	else {
		tagMissing = true;
        if (cover.image.getWidth() == 2)
        {
            multiTrack = true;
        }
		repaint();
	}
	
	
}

void AlbumArt::setCover (ImageWithType cover)
{
	if (cover.image.isValid() && cover.image.getWidth() > 2) {
		fileSelected = true;
        tagMissing = false;
        
        imageWidth = cover.image.getWidth();
        imageHeight = cover.image.getHeight();
        
        if(findParentComponentOfClass<Viewport>())
        {
            setSize(getParentWidth(), getParentHeight());
        }
        
        if (audioFile.existsAsFile())
        {
            currentCover = cover.image;
            extension = cover.type;
            
            setImage (cover.image);
            
            //If parent is the guicontrol display then it saves when the image is set, otherwise save is called from another function when ok is pressed.
            GuiControl* gc = findParentComponentOfClass<GuiControl>();
            if (gc != nullptr)
                TagReader::saveAlbumArt(audioFile, currentCover, extension);
        }
	}
	else {
		tagMissing = true;
        if (cover.image.getWidth() == 2)
        {
            multiTrack = true;
        }
		repaint();
	}
	
	
}

void AlbumArt::setCover (Image cover)
{
	if (cover.isValid() && cover.getWidth() > 2) {
		fileSelected = true;
        tagMissing = false;
        
        imageWidth = cover.getWidth();
        imageHeight = cover.getHeight();
        
        if(findParentComponentOfClass<Viewport>())
        {
            setSize(getParentWidth(), getParentHeight());
        }
        
        if (audioFile.existsAsFile())
        {
            currentCover = cover;
            
            setImage (cover);
            
            //If parent is the guicontrol display then it saves when the image is set, otherwise save is called from another function when ok is pressed.
            GuiControl* gc = findParentComponentOfClass<GuiControl>();
            if (gc != nullptr)
                TagReader::saveAlbumArt(audioFile, cover, extension);
        }
	}
	else {
		tagMissing = true;
        if (cover.getWidth() == 2)
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
    popupMenu.addItem(1, "Copy");
    if (artClipboard.type != "")
    {
        popupMenu.addItem(2, "Paste");
    }
    popupMenu.addSeparator();
    popupMenu.addItem(3,"Add from File",true);
    popupMenu.addItem(4,"Add from URL",true);
    
    int menuResult = popupMenu.show();
    
    switch (menuResult) {
        case 1:
        {
            artClipboard.image = currentCover;
            artClipboard.type = extension;
            break;
        }
        case 2:
            setCover(artClipboard);
            break;
        case 3:
            fromFile();
            break;
        case 4:
            fromUrl();
            break;
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
        
        if (!multiTrack) {
            extension = fc.getResult().getFileExtension();
            if (extension.compareIgnoreCase(".jpeg") || extension.compareIgnoreCase(".jpg"))
                TagReader::saveAlbumArt(audioFile, currentCover, "JPEG");
            else
                TagReader::saveAlbumArt(audioFile, currentCover, "PNG");
        }

        artUpdateRequired = true;
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
        if (urlLoad.jpeg)
            extension = "jpg";
        else
            extension = "png";
        
        setCover(currentCover);
//        if (!multiTrack) {
//            if (urlLoad.jpeg)
//                setCover()
//            else
//                TagReader::saveAlbumArt(audioFile, currentCover, "PNG"); 
        
//        }
        
        artUpdateRequired = true;
    }
    
    urlAlert.removeCustomComponent(0);
}
