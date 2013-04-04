/*
 *  TrackSummary.cpp
 *  MusicPlayer
 *
 *  Created by Andy on 29/11/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "TrackSummary.h"

TrackSummary::TrackSummary ()
{
    addAndMakeVisible (&albumArt);
    
    addAndMakeVisible (&artist);
    artist.setFont (Font (20.0000f, Font::plain));
    
    addAndMakeVisible (&album);
    album.setFont (Font (20.0000f, Font::plain));
    
    
    addAndMakeVisible (&song);
    song.setFont (Font (20.0000f, Font::plain));
    
    
    addAndMakeVisible (&locationLabel);
    locationLabel.setText("Location: ", dontSendNotification);
    
    addAndMakeVisible (&location);
    
    addAndMakeVisible (&typeLabel);
    typeLabel.setText("Type: ", dontSendNotification);
    
    addAndMakeVisible (&type);
    
    addAndMakeVisible (&sizeLabel);
    sizeLabel.setText("Size: ", dontSendNotification);
    
    addAndMakeVisible (&size);
    
    
    addAndMakeVisible (&addedLabel);
    addedLabel.setText("Added: ", dontSendNotification);
    
    addAndMakeVisible (&added);
    
    
    addAndMakeVisible (&modifiedLabel);
    modifiedLabel.setText("Modified: ", dontSendNotification);
    
    addAndMakeVisible (&modified);
    
    
    addAndMakeVisible (&bitLabel);
    bitLabel.setText("Bit Rate: ", dontSendNotification);
    
    addAndMakeVisible (&bitRate);
    
    
    addAndMakeVisible (&sampleLabel);
    sampleLabel.setText("Sample Rate: ", dontSendNotification);
    
    addAndMakeVisible (&sampleRate);
    
    
    setSize (530, 510);
}

TrackSummary::~TrackSummary()
{
    
}

//==============================================================================
void TrackSummary::paint (Graphics& g)
{
    g.fillAll (Colours::white);
    
    g.setColour (Colours::dimgrey);
    g.fillRect (0, 212, 530, 1);
}

void TrackSummary::resized()
{
    artist.setBounds (176, 60, 544, 24);
    album.setBounds (176, 90, 544, 24);
    song.setBounds (176, 30, 544, 24);
    
    albumArt.setBounds (5, 5, 160, 160);
    
    locationLabel.setBounds (15, 215, 72, 24);
    location.setBounds (20, 230, 500, 50);
    
    typeLabel.setBounds (15, 280, 48, 24);
    type.setBounds (40, 295, 160, 24);
    
    sizeLabel.setBounds (15, 320, 48, 24);
    size.setBounds (40, 335, 64, 24);
    
    addedLabel.setBounds (15, 360, 64, 24);
    added.setBounds (40, 380, 160, 24);
    
    modifiedLabel.setBounds (15, 400, 80, 24);
    modified.setBounds (40, 420, 160, 24);
    
    bitLabel.setBounds (300, 280, 80, 24);
    bitRate.setBounds (325, 295, 80, 24);
    
    sampleLabel.setBounds (300, 320, 104, 24);
    sampleRate.setBounds (325, 335, 100, 24);
}

void TrackSummary::setTrack(ValueTree incomingTrack)
{
    File selectedFile (incomingTrack.getProperty(MusicColumns::columnNames[MusicColumns::Location]));
    
    artist.setText (incomingTrack.getProperty(MusicColumns::columnNames[MusicColumns::Artist]).toString(), dontSendNotification);
    albumArt.setCover(selectedFile);
    
    album.setText (incomingTrack.getProperty(MusicColumns::columnNames[MusicColumns::Album]).toString(), dontSendNotification);
    
    song.setText (incomingTrack.getProperty(MusicColumns::columnNames[MusicColumns::Song]).toString(), dontSendNotification);
    
    location.setText (incomingTrack.getProperty(MusicColumns::columnNames[MusicColumns::Location]).toString(), dontSendNotification);
    
    type.setText (incomingTrack.getProperty(MusicColumns::columnNames[MusicColumns::Kind]).toString(), dontSendNotification);
    
    size.setText(File::descriptionOfSizeInBytes(incomingTrack.getProperty(MusicColumns::columnNames[MusicColumns::Size])), dontSendNotification);
    
    String addedString = Time (int64 (incomingTrack[MusicColumns::columnNames[MusicColumns::Added]])).formatted ("%d/%m/%Y - %H:%M");
    added.setText (addedString, dontSendNotification);
    
    String modifiedString = Time (int64 (incomingTrack[MusicColumns::columnNames[MusicColumns::Modified]])).formatted ("%d/%m/%Y - %H:%M");
    modified.setText (modifiedString, dontSendNotification);

    int bitRateInt = incomingTrack[MusicColumns::columnNames[MusicColumns::BitRate]];
    String bitRateString (String(bitRateInt) + " kbps");
    bitRate.setText(bitRateString, dontSendNotification);
    
    int sampleRateInt = incomingTrack[MusicColumns::columnNames[MusicColumns::SampleRate]];
    String sampleRateString (String(sampleRateInt) + " Hz");
    sampleRate.setText(sampleRateString, dontSendNotification);
}