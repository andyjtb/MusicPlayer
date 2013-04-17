/*
 *  GuiControl.h
 *  MusicPlayer
 *
 *  Created by Andy on 26/10/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef H_GUICONTROL
#define H_GUICONTROL

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioControl.h"

#include "PlayButton.h"
#include "BackwardsButton.h"
#include "ForwardButton.h"

#include "OutputMeter.h"
#include "VolumeControl.h"
#include "TransportSlider.h"

#include "TrackInfo.h"
#include "AlbumArt.h"
#include "Settings.h"

#include "InfoBar.h"

#include "MusicLibraryTableModel.h"
#include "MusicLibraryHelpers.h"

#include "Comparators.h"

#include "SearchBox.h"
#include "MusicLibraryDropTarget.h"

#include "LibraryTreeView.h"

#include "Equaliser.h"
#include "SpeedPitch.h"

class RemoteInterprocessConnection;

/**

 */
class GuiControl  : public Component,
					public ActionListener,
					public ChangeListener,
					public ValueListener,
					public MultiTimer,
                    public TextEditor::Listener,
                    public ValueTree::Listener
{
public:
	//==============================================================================
    /**
     Constructor
     */
	GuiControl();
    
    /**
     Destructor
     */
	~GuiControl();
    
    void paint(Graphics& g);
	void resized();
	
	void setAudioControl (AudioControl* incomingAudioControl);
	
	void actionListenerCallback (const String& message);
	
	void valueChanged (Value& valueChanged);

	void timerCallback(int timerId);
	
	void updateTagDisplay (File audioFile);
	
	void changeListenerCallback(ChangeBroadcaster* changeBroadcaster);
    
    void loadFile();
    
    //Track control
    void next();
    void previous();
    
    //Remote 
    void setVolume(double incomingVolume);
    void setPosition(double incomingPosition);
    
    //Text Editor Callbacks
    void textEditorTextChanged (TextEditor &textEditor);
    void textEditorReturnKeyPressed (TextEditor &textEditor);
    void textEditorEscapeKeyPressed (TextEditor &textEditor);
    void textEditorFocusLost (TextEditor &textEditor);
	
    //ValueTree Callbacks
    void valueTreePropertyChanged (ValueTree &treeWhosePropertyHasChanged, const Identifier &property);
    void valueTreeChildAdded (ValueTree &parentTree, ValueTree &childWhichHasBeenAdded);
    void valueTreeChildRemoved (ValueTree &parentTree, ValueTree &childWhichHasBeenRemoved);
    void valueTreeChildOrderChanged (ValueTree &parentTreeWhoseChildrenHaveMoved);
    void valueTreeParentChanged (ValueTree &treeWhoseParentHasChanged);
    void valueTreeRedirected (ValueTree &treeWhichHasBeenChanged);

    //Menu Bar function
    void showEffectsMenu();
    
    void remoteConnectionChanged(String ipAddress) { infoBar.getConnectionIndicator().remoteConnectionChanged(ipAddress);}
    
    void setPlaylist (String incomingPlaylist);
private:
	AudioControl* audioControl;
	
	PlayButton playButton;
	VolumeControl volumeControl;
	OutputMeter outputMeters;
	TransportSlider transport;
	
    TrackInfo trackInfo;
	AlbumArt albumArt;
    
    InfoBar infoBar;
    
    MusicLibraryTable* musicTable;
    
    SearchBox searchBox;
    
    MusicLibraryDropTarget musicLibraryDropTarget;

    LibraryTreeView libraryView;
//    CoverFlowComponent* coverflow;
//Effects pointers
    ScopedPointer<TabbedComponent> effectsTabbed;
    ScopedPointer<Equaliser> equaliser;
    ScopedPointer<SpeedPitch> speedPitch;
};



#endif //H_GUICONTROL