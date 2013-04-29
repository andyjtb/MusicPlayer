/*
 *  TrackInfo.cpp
 *  sdaMidiMeter
 *
 *  Created by Andy Brown on 12/8/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "TrackInfo.h"

TrackInfo::TrackInfo ()
{
	addAndMakeVisible(&artist);
	artist.setText("", dontSendNotification);
    artist.setColour (Label::textColourId, Colour (0xff6e7075));
    artist.setFont (Font ("Helvetica Neue", 15.0000f, Font::plain));
    artist.setText ("Artist",dontSendNotification);
    
	addAndMakeVisible(&title);
	title.setText("", dontSendNotification);
    title.setColour (Label::textColourId, Colours::black);
    title.setFont (Font ("Helvetica Neue", 15.0000f, Font::plain));
    title.setText ("Song Title",dontSendNotification);
    
    addAndMakeVisible(&album);
    album.setText("", dontSendNotification);
    album.setColour (Label::textColourId, Colour (0xff6e7075));
    album.setFont (Font ("Helvetica Neue", 15.0000f, Font::plain));		
    album.setText("Album", dontSendNotification);
}

TrackInfo::~TrackInfo()
{

}

void TrackInfo::resized()
{
    title.setBounds (0, 0, getWidth(), 20);
    artist.setBounds (0, 25, getWidth(), 20);
    album.setBounds (0, 45, getWidth(), 20);

}

void TrackInfo::loadTrackInfo(ValueTree tags)
{	
   if (tags.hasType("ITEM")) {
	   title.setText (tags.getProperty(MusicColumns::columnNames[MusicColumns::Song].toString(), 0),dontSendNotification);
	   artist.setText (tags.getProperty(MusicColumns::columnNames[MusicColumns::Artist].toString(), 0),dontSendNotification);
	   album.setText (tags.getProperty(MusicColumns::columnNames[MusicColumns::Album].toString(), 0),dontSendNotification);
   }
	else {
		title.setText ("Song Title",dontSendNotification);
		artist.setText ("Artist",dontSendNotification);
        album.setText("Album", dontSendNotification);
	}
}

