/*
 *  TrackSummary.cpp
 *  MusicPlayer
 *
 *  Created by Andy on 29/11/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "TrackSummary.h"

TrackSummary::TrackSummary (int incomingTree)
{
    File selectedFile (singletonLibraryTree.getChild(incomingTree).getProperty(MusicColumns::columnNames[MusicColumns::Location]));
    ValueTree songTree (singletonLibraryTree.getChild(incomingTree));
    
    addAndMakeVisible (&artist);
    artist.setFont (Font (20.0000f, Font::plain));
    artist.setText (songTree.getProperty(MusicColumns::columnNames[MusicColumns::Artist]).toString(), false);

    addAndMakeVisible (&albumArt);
    albumArt.setCover(TagReader::getAlbumArt(selectedFile));

    addAndMakeVisible (&album);
    album.setFont (Font (20.0000f, Font::plain));
    artist.setText (songTree.getProperty(MusicColumns::columnNames[MusicColumns::Album]).toString(), false);
    
    addAndMakeVisible (&song);
    song.setFont (Font (20.0000f, Font::plain));
    song.setText (songTree.getProperty(MusicColumns::columnNames[MusicColumns::Song]).toString(), false);

    addAndMakeVisible (&locationLabel);
    locationLabel.setText("Location: ", false);

    addAndMakeVisible (&location);
    location.setText (songTree.getProperty(MusicColumns::columnNames[MusicColumns::Location]).toString(), false);

    addAndMakeVisible (&typeLabel);
    typeLabel.setText("Type: ", false);

    addAndMakeVisible (&type);
    type.setText (songTree.getProperty(MusicColumns::columnNames[MusicColumns::Kind]).toString(), false);

    addAndMakeVisible (&sizeLabel);
    sizeLabel.setText("Size: ", false);

    addAndMakeVisible (&size);
    size.setText(File::descriptionOfSizeInBytes(selectedFile.getSize()), false);

    addAndMakeVisible (&addedLabel);
    addedLabel.setText("Added: ", false);

    addAndMakeVisible (&added);
    added.setText (songTree.getProperty(MusicColumns::columnNames[MusicColumns::Added]).toString(), false);

    addAndMakeVisible (&modifiedLabel);
    modifiedLabel.setText("Modified: ", false);

    addAndMakeVisible (&modified);
    modified.setText (songTree.getProperty(MusicColumns::columnNames[MusicColumns::Modified]).toString(), false);

    AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    
    ScopedPointer<AudioFormatReader> reader;
    reader = formatManager.createReaderFor (selectedFile);
    
    addAndMakeVisible (&bitLabel);
    bitLabel.setText("Bit Rate: ", false);

    addAndMakeVisible (&bitRate);
    String bitRateString (reader->bitsPerSample + " kbps");
    bitRate.setText(bitRateString, false);

    addAndMakeVisible (&sampleLabel);
    sampleLabel.setText("Sample Rate: ", false);

    addAndMakeVisible (&sampleRate);
    String sampleRateString (String(reader->sampleRate) + " Hz");
    sampleRate.setText(sampleRateString, false);


    setSize (600, 510);
}

TrackSummary::~TrackSummary()
{

}

//==============================================================================
void TrackSummary::paint (Graphics& g)
{
    g.fillAll (Colours::white);

    g.setColour (Colours::dimgrey);
    g.fillRect (4, 212, 600, 1);
}

void TrackSummary::resized()
{
    artist.setBounds (176, 56, 544, 24);
    albumArt.setBounds (16, 16, 152, 144);
    album.setBounds (176, 80, 544, 24);
    song.setBounds (176, 32, 544, 24);
    locationLabel.setBounds (16, 216, 72, 24);
    location.setBounds (40, 240, 616, 24);
    typeLabel.setBounds (16, 272, 48, 24);
    type.setBounds (40, 296, 160, 24);
    sizeLabel.setBounds (16, 328, 48, 24);
    size.setBounds (40, 352, 64, 24);
    addedLabel.setBounds (16, 384, 64, 24);
    added.setBounds (40, 408, 120, 24);
    modifiedLabel.setBounds (16, 440, 80, 24);
    modified.setBounds (40, 472, 120, 24);
    bitLabel.setBounds (424, 272, 80, 24);
    bitRate.setBounds (456, 296, 48, 24);
    sampleLabel.setBounds (424, 328, 104, 24);
    sampleRate.setBounds (456, 352, 48, 24);
}
