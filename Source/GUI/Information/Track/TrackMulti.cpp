//
//  TrackMulti.cpp
//  MusicPlayer
//
//  Created by Andy on 03/12/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "TrackMulti.h"

TrackMulti::TrackMulti(Array<int> incomingIds)
{    
    libraryIds = incomingIds;
    
    addAndMakeVisible (&artistLabel);
    artistLabel.setText("Artist: ", false);
    
    addAndMakeVisible (&artist);
    artist.setMultiLine (false);
    artist.setReturnKeyStartsNewLine (false);
    artist.setReadOnly (false);
    artist.setScrollbarsShown (false);
    artist.setCaretVisible (true);
    artist.setPopupMenuEnabled (true);
//    artist.addListener(this);
    
    addAndMakeVisible (&albumLabel);
    albumLabel.setText("Album: ", false);
    
    addAndMakeVisible (&album);
    album.setMultiLine (false);
    album.setReturnKeyStartsNewLine (false);
    album.setReadOnly (false);
    album.setScrollbarsShown (false);
    album.setCaretVisible (true);
    album.setPopupMenuEnabled (true);
//    album.addListener(this);
    
    addAndMakeVisible (&genreLabel);
    genreLabel.setText("Genre: ", false);
    
    addAndMakeVisible (&genre);
    genre.setMultiLine (false);
    genre.setReturnKeyStartsNewLine (false);
    genre.setReadOnly (false);
    genre.setScrollbarsShown (false);
    genre.setCaretVisible (true);
    genre.setPopupMenuEnabled (true);
//    genre.addListener(this);
    
    addAndMakeVisible (&ratingLabel);
    ratingLabel.setText("Rating: ", false);
    
    addAndMakeVisible (&rating);
    rating.setRange (0, 5, 1);
    rating.setSliderStyle (Slider::IncDecButtons);
    rating.setTextBoxStyle (Slider::TextBoxLeft, false, 30, 20);
//    rating.addListener(this);
    
    addAndMakeVisible (&labelLabel);
    labelLabel.setText("Label: ", false);
    
    addAndMakeVisible (&label);
    label.setMultiLine (true);
    label.setReturnKeyStartsNewLine (true);
    label.setReadOnly (false);
    label.setScrollbarsShown (true);
    label.setCaretVisible (true);
    label.setPopupMenuEnabled (true);
//    label.addListener(this);
    
    addAndMakeVisible(&artLabel);
    artLabel.setText("Album Art: ", false);
    
    addAndMakeVisible(&art);
    
    setInfo();
    
    setSize (470, 400);
    
}

TrackMulti::~TrackMulti()
{
    
}

//==============================================================================
void TrackMulti::paint (Graphics& g)
{

}

void TrackMulti::resized()
{
    artistLabel.setBounds (0, 0, 64, 24);
    artist.setBounds (16, 24, 296, 24);
    albumLabel.setBounds (0, 48, 64, 24);
    album.setBounds (16, 72, 296, 24);
    genreLabel.setBounds (0, 96, 64, 24);
    genre.setBounds (16, 120, 296, 24);
    ratingLabel.setBounds (88, 144, 64, 24);
    rating.setBounds (128, 168, 96, 24);
    labelLabel.setBounds (0, 200, 48, 24);
    label.setBounds (16, 224, 440, 120);
    artLabel.setBounds(334, 0, 84, 24);
    art.setBounds(340, 24, 125, 125);
}


void TrackMulti::setInfo()
{
    
    
    ValueTree firstTrack = filteredDataList.getChildWithProperty(MusicColumns::columnNames[MusicColumns::LibID], libraryIds[0]);
    
    String artistString = firstTrack.getProperty(MusicColumns::columnNames[MusicColumns::Artist]).toString();
    String albumString = firstTrack.getProperty(MusicColumns::columnNames[MusicColumns::Album]).toString();
    String genreString = firstTrack.getProperty(MusicColumns::columnNames[MusicColumns::Genre]).toString();
    String labelString = firstTrack.getProperty(MusicColumns::columnNames[MusicColumns::Label]).toString();
    int ratingNum = firstTrack.getProperty(MusicColumns::columnNames[MusicColumns::Rating]);

    File selectedFile (firstTrack.getProperty(MusicColumns::columnNames[MusicColumns::Location]));
    
    bool artistMatch, albumMatch, genreMatch, labelMatch, ratingMatch, artMatch;
    artistMatch = albumMatch = genreMatch = labelMatch = ratingMatch = artMatch = true;
    
    for (int counter = 0; counter < libraryIds.size(); counter++) {
        
        ValueTree currentTrack = filteredDataList.getChildWithProperty(MusicColumns::columnNames[MusicColumns::LibID], libraryIds[counter]);
        
        String currentArtist = currentTrack.getProperty(MusicColumns::columnNames[MusicColumns::Artist]).toString();
        String currentAlbum = currentTrack.getProperty(MusicColumns::columnNames[MusicColumns::Album]).toString();
        String currentGenre = currentTrack.getProperty(MusicColumns::columnNames[MusicColumns::Genre]).toString();
        String currentLabel = currentTrack.getProperty(MusicColumns::columnNames[MusicColumns::Label]).toString();
        int currentRating = currentTrack.getProperty(MusicColumns::columnNames[MusicColumns::Rating]);
        
        File currentFile (currentTrack.getProperty(MusicColumns::columnNames[MusicColumns::Location]));
        
        if (artistString != currentArtist)
        {
            artistMatch = false;
        }
        if (albumString != currentAlbum)
        {
            albumMatch = false;
        }
        if (genreString != currentGenre)
        {
            genreMatch = false;
        }
        if (labelString != currentLabel)
        {
            labelMatch = false;
        }
        if (ratingNum != currentRating)
        {
            ratingMatch = false;
        }
        
        if (!TagReader::compareAlbumArt(selectedFile, currentFile))
        {
            DBG("ART DOESNT MATCH");
            artMatch = false;
        }
    }
    
    if (artistMatch)
    {
        artist.setText(artistString);
    }
    if (albumMatch)
    {
        album.setText(albumString);
    }
    if (genreMatch)
    {
        genre.setText(genreString);
    }
    if (labelMatch)
    {
        label.setText(labelString);
    }
    if (ratingMatch)
    {
        rating.setValue(ratingNum);
    }
    
    if (artMatch)
    {
        art.setCover(selectedFile);
    }
    else
    {
        Image doesntMatch (Image::RGB, 2,2,true);
        art.setCover(doesntMatch);                   
    }
}

void TrackMulti::saveEdits ()
{
    newArt = art.getCover();
    
    for (int counter = 0; counter < libraryIds.size(); counter++) {
        ValueTree currentTrack = filteredDataList.getChildWithProperty(MusicColumns::columnNames[MusicColumns::LibID], libraryIds[counter]);
        
        if (artist.getText() != String::empty)
        {
            currentTrack.setProperty(MusicColumns::columnNames[MusicColumns::Artist], artist.getText(), singletonUndoManager);
            TagReader::writeTag(MusicColumns::Artist, currentTrack);
        }
        if (album.getText() != String::empty)
        {
            currentTrack.setProperty(MusicColumns::columnNames[MusicColumns::Album], album.getText(), singletonUndoManager);
            TagReader::writeTag(MusicColumns::Album, currentTrack);
        }
        if (genre.getText() != String::empty)
        {
            currentTrack.setProperty(MusicColumns::columnNames[MusicColumns::Genre], genre.getText(), singletonUndoManager);
            TagReader::writeTag(MusicColumns::Genre, currentTrack);
        }
        if (label.getText() != String::empty)
        {
            currentTrack.setProperty(MusicColumns::columnNames[MusicColumns::Label], label.getText(), singletonUndoManager);
            TagReader::writeTag(MusicColumns::Label, currentTrack);
        }
        if (rating.getValue() > 0)
        {
            currentTrack.setProperty(MusicColumns::columnNames[MusicColumns::Rating], rating.getValue(), singletonUndoManager);
            TagReader::writeTag(MusicColumns::Rating, currentTrack);
        }
        
        if (newArt.isValid() && newArt.getWidth() >= 3)
        {
            File currentFile (currentTrack.getProperty(MusicColumns::columnNames[MusicColumns::Location]));
            //save album art
            TagReader::saveAlbumArt (currentFile, newArt, art.extension);
            artUpdateRequired = true;
        }
        
        currentTrack.setProperty(MusicColumns::columnNames[MusicColumns::Modified], Time::getCurrentTime().toMilliseconds(), 0);
        
        
        File selectedFile (currentTrack.getProperty(MusicColumns::columnNames[MusicColumns::Location]));
        selectedFile.setLastModificationTime(Time::getCurrentTime());
        
    }
    
}

