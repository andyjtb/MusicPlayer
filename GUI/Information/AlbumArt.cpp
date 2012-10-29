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

    setSize (175, 175);
	
}

AlbumArt::~AlbumArt()
{
	
}

//==============================================================================

void AlbumArt::resized()
{
	setBounds (0, 0, 175, 175);
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
					0, 0, 175, 175,
					Justification::centred, true);
		fileSelected = true;
		tagMissing = false;
	}
}

void AlbumArt::setCover (Image cover)
{
	if (cover.isValid()) {
		fileSelected = true;
		setImage (cover);
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
		//		if (menuResult == 2) {
		//			int result=0;
		//			UrlLoad urlLoad;
		//			AlertWindow urlAlert("Choose an album cover to open...", "You picked: ", AlertWindow::NoIcon);
		//			
		//			urlAlert.addCustomComponent(&urlLoad);
		//			urlAlert.addButton("Cancel", 0);
		//			urlAlert.addButton("Ok", 1);
		//			result = urlAlert.runModalLoop();
		//			urlAlert.removeCustomComponent(0);
		//			
		//			if (result == 0) {
		//				std::cout<< "cancelled";
		//			}
		//			if (result==1) {
		//				std::cout << "Ok";
		//				newCover = urlLoad.getImage();
		//				setImages (false, true, true,
		//						   newCover, 1.0000f, Colour (0x0),
		//						   newCover, 1.0000f, Colour (0x0),
		//						   newCover, 1.0000f, Colour (0x0));
		//				sendActionMessage("albumart");
		//			}
		//			
		//		}
		//		
	}
}	
