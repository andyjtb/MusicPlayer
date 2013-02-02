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
	addAndMakeVisible(&lTitle);
	lTitle.setText("Song Title", false);

	addAndMakeVisible(&lArtist);
	lArtist.setText("Artist", false);

	addAndMakeVisible(&lId3);
	lId3.setText("ID3 Tag Check :", false);

	addAndMakeVisible(&artist);
	artist.setText("Artist", false);

	addAndMakeVisible(&title);
	title.setText("Song Title", false);

	addAndMakeVisible(&id3);
	id3.setText("Waiting...", false);

    addAndMakeVisible (&toggleId3);
	toggleId3.setClickingTogglesState (false);

    setSize (274, 150);
}

TrackInfo::~TrackInfo()
{

}

void TrackInfo::resized()
{
    lTitle.setBounds (0, 0, 80, 24);
    lArtist.setBounds (2, 48, 46, 24);
    lId3.setBounds (8, 104, 104, 24);
    artist.setBounds (8, 65, 256, 24);
    title.setBounds (8, 16, 264, 24);
    id3.setBounds (112, 104, 104, 24);
    toggleId3.setBounds (32, 120, 24, 32);

}

void TrackInfo::loadTrackInfo(ValueTree tags)
{	
   if (tags.hasType("ITEM")) {
	   toggleId3.setToggleState(true,false);
	   title.setText (tags.getProperty(MusicColumns::columnNames[MusicColumns::Song].toString(), 0),false);
	   artist.setText (tags.getProperty(MusicColumns::columnNames[MusicColumns::Artist].toString(), 0),false);
	   id3.setText ("Tag Loaded",false);
	   
   }
	else {
		toggleId3.setToggleState(false,false);
		title.setText ("Failed",false);
		artist.setText ("Failed",false);
		id3.setText ("Tag Not Found",false);
	}
}

