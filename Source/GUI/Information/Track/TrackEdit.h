//
//  TrackEdit.h
//  MusicPlayer
//
//  Created by Andy on 03/12/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef TRACKEDIT
#define TRACKEDIT

#include "../JuceLibraryCode/JuceHeader.h"
#include "Settings.h"
#include "MusicLibraryHelpers.h"
#include "TagReader.h"
/** The display that allows the user to edit the metadata about a track. For multiple tracks see TrackMulti */
class TrackEdit  :	public Component,
                    public Slider::Listener,
                    public TextEditor::Listener
{
public:
    /** Constructor */
    TrackEdit ();
    /** Destrucot */
    ~TrackEdit();
	
    /** @internal */
    void resized();
    /** @internal */
	void paint(Graphics& g);
	/** Loads the track info into the TextEditor s and AlbumArt components */
    void setTrack(ValueTree incomingTrack);
    /** Saves all of the information about the track, provided atleast one of the pieces of information has changed - Does not save album art as this is dealt with internally by the AlbumArt class*/
    void saveEdits();
    
    /** Text editor callback - When one of the editors changes it sets the saveRequired bool to true, meaning all changes will be saved when the window closes or next/previous is pressed */
    void textEditorTextChanged (TextEditor &textEditor);
    /** @internal */
    void textEditorReturnKeyPressed (TextEditor &textEditor) {}
    /** @internal */
    void textEditorEscapeKeyPressed (TextEditor &textEditor) {}
    /** @internal */
    void textEditorFocusLost (TextEditor &textEditor) {}
    /** Slider callback - When one of the sliders changes it sets the saveRequired bool to true, meaning all changes will be saved when the window closes or next/previous is pressed*/
    void sliderValueChanged (Slider* sliderThatWasMoved);
    
private:
    Label songLabel;
    TextEditor song;
    Label artistLabel;
    TextEditor artist;
    Label albumLabel;
    TextEditor album;
    Label genreLabel;
    TextEditor genre;
    Label ratingLabel;
    Slider rating;
    Label labelLabel;
    TextEditor label;
    Label trackLabel;
    TextEditor trackNum;
    
    ValueTree songTree;
    
    bool saveRequired, settingInfo;

};


#endif
