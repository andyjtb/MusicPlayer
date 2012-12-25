//
//  TrackEdit.cpp
//  MusicPlayer
//
//  Created by Andy on 03/12/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "TrackEdit.h"

TrackEdit::TrackEdit()
{    
    saveRequired = false;
    addAndMakeVisible (&songLabel);
    songLabel.setText("Song: ", false);
    
    addAndMakeVisible (&song);
    song.setMultiLine (false);
    song.setReturnKeyStartsNewLine (false);
    song.setReadOnly (false);
    song.setScrollbarsShown (false);
    song.setCaretVisible (true);
    song.setPopupMenuEnabled (true);
//    song.addListener(this);
    
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
    
    addAndMakeVisible (&trackLabel);
    trackLabel.setText("Track Number: ", false);
    
    addAndMakeVisible (&trackNum);
    trackNum.setInputRestrictions(4, "0123456789");
    trackNum.setMultiLine (false);
    trackNum.setReturnKeyStartsNewLine (false);
    trackNum.setReadOnly (false);
    trackNum.setScrollbarsShown (false);
    trackNum.setCaretVisible (true);
    trackNum.setPopupMenuEnabled (false);    
//    trackNum.addListener(this);
    
    setSize (530, 510);
    
}

TrackEdit::~TrackEdit()
{
    
}

//==============================================================================
void TrackEdit::paint (Graphics& g)
{

}

void TrackEdit::resized()
{
    songLabel.setBounds (0, 0, 48, 24);
    song.setBounds (16, 24, 296, 24);
    artistLabel.setBounds (0, 48, 64, 24);
    artist.setBounds (16, 72, 296, 24);
    albumLabel.setBounds (0, 96, 64, 24);
    album.setBounds (16, 120, 296, 24);
    genreLabel.setBounds (0, 144, 64, 24);
    genre.setBounds (16, 168, 296, 24);
    ratingLabel.setBounds (88, 200, 64, 24);
    rating.setBounds (128, 224, 96, 24);
    labelLabel.setBounds (0, 248, 48, 24);
    label.setBounds (16, 272, 440, 120);
    trackLabel.setBounds (360, 56, 112, 24);
    trackNum.setBounds (384, 80, 56, 24);
    //save.setBounds (192, 432, 150, 24);
}


void TrackEdit::setTrack(ValueTree incomingTrack)
{
    File selectedFile (incomingTrack.getProperty(MusicColumns::columnNames[MusicColumns::Location]));
    songTree = incomingTrack;
    
    artist.setText (songTree.getProperty(MusicColumns::columnNames[MusicColumns::Artist]).toString(), false);
    
    album.setText (songTree.getProperty(MusicColumns::columnNames[MusicColumns::Album]).toString(), false);
    
    song.setText (songTree.getProperty(MusicColumns::columnNames[MusicColumns::Song]).toString(), false);
    
    genre.setText (songTree.getProperty(MusicColumns::columnNames[MusicColumns::Genre]).toString(), false);
    
    label.setText (songTree.getProperty(MusicColumns::columnNames[MusicColumns::Label]).toString(), false);
    
    rating.setValue(songTree.getProperty(MusicColumns::columnNames[MusicColumns::Rating]));
    
    trackNum.setText(songTree.getProperty(MusicColumns::columnNames[MusicColumns::TrackNum]));
    
    saveRequired = false;
    
    song.addListener(this);
    artist.addListener(this);
    album.addListener(this);
    genre.addListener(this);
    rating.addListener(this);
    label.addListener(this);
    trackNum.addListener(this);
}

void TrackEdit::saveEdits ()
{
    if (saveRequired)
    {
        File selectedFile (songTree.getProperty(MusicColumns::columnNames[MusicColumns::Location]));
        
        songTree.setProperty(MusicColumns::columnNames[MusicColumns::Song], song.getText(), 0);
        
        songTree.setProperty(MusicColumns::columnNames[MusicColumns::Artist], artist.getText(), 0);
        
        songTree.setProperty(MusicColumns::columnNames[MusicColumns::Album], album.getText(), 0);
        
        songTree.setProperty(MusicColumns::columnNames[MusicColumns::Genre], genre.getText(), 0);
        
        songTree.setProperty(MusicColumns::columnNames[MusicColumns::Label], label.getText(), 0);
        if (rating.getValue() > 0)
        {
            songTree.setProperty(MusicColumns::columnNames[MusicColumns::Rating], rating.getValue(), 0);
        }
        
        songTree.setProperty(MusicColumns::columnNames[MusicColumns::Modified], Time::getCurrentTime().toMilliseconds(), 0);
        selectedFile.setLastModificationTime(Time::getCurrentTime());
        
        songTree.setProperty(MusicColumns::columnNames[MusicColumns::TrackNum], trackNum.getText().getIntValue(), 0);
        
        TagReader::saveTags(songTree);
        
        saveRequired = false;
    }
    song.removeListener(this);
    artist.removeListener(this);
    album.removeListener(this);
    genre.removeListener(this);
    rating.removeListener(this);
    label.removeListener(this);
    trackNum.removeListener(this);
}

//Text editor callbacks
void TrackEdit::textEditorTextChanged (TextEditor &textEditor)
{
    saveRequired = true;
}
void TrackEdit::textEditorReturnKeyPressed (TextEditor &textEditor)
{}
void TrackEdit::textEditorEscapeKeyPressed (TextEditor &textEditor)
{}
void TrackEdit::textEditorFocusLost (TextEditor &textEditor)
{}

void TrackEdit::sliderValueChanged (Slider* sliderThatWasMoved)
{
	saveRequired = true;
}
