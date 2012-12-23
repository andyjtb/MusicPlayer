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

class TrackEdit  :	public Component,
                    public Slider::Listener,
                    public TextEditor::Listener
{
public:
    TrackEdit ();
    ~TrackEdit();
	
    void resized();
	void paint(Graphics& g);
	
    void setTrack(ValueTree incomingTrack);

    void saveEdits();
    
    void textEditorTextChanged (TextEditor &textEditor);
    void textEditorReturnKeyPressed (TextEditor &textEditor);
    void textEditorEscapeKeyPressed (TextEditor &textEditor);
    void textEditorFocusLost (TextEditor &textEditor);
    void sliderValueChanged (Slider* sliderThatWasMoved);
    
    void setSaveRequired (bool incomingRequired);
    
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
    TextButton save;
    
    ValueTree songTree;
    
    bool saveRequired, settingInfo;

};


#endif
