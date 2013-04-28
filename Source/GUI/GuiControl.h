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
                    public TextEditor::Listener
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
	AudioControl* getAudioControl () { return audioControl; }
    
	void actionListenerCallback (const String& message);
	
	void valueChanged (Value& valueChanged);

	void timerCallback(int timerId);
	
	void updateSelectedDisplay ();
	
	void changeListenerCallback(ChangeBroadcaster* changeBroadcaster);
    
    void loadFile(ValueTree treeToLoad, bool shouldPlay);
    
    //Track control
    void next();
    void previous();
    
    //Remote 
    void setVolume(double incomingVolume);
    void setPosition(double incomingPosition);
    
    //Text Editor Callbacks
    void textEditorTextChanged (TextEditor &textEditor);
//    /** Not needed, but required
//     */
//    void textEditorReturnKeyPressed (TextEditor &textEditor){}
//    /** Not needed, but required
//     */
//    void textEditorEscapeKeyPressed (TextEditor &textEditor){}
//    /** Not needed, but required
//     */
//    void textEditorFocusLost (TextEditor &textEditor){}

    ValueTree getCurrentlyPlayingList() { return currentlyPlayingList; }
    
    //Menu Bar function
    void showEffectsMenu();
    
    InfoBar* getInfoBar() { return &infoBar; }
    void remoteConnectionChanged(String ipAddress) { infoBar.getConnectionIndicator().remoteConnectionChanged(ipAddress);}
    
    void setPlaylist (String incomingPlaylist);
    String getPlaylist() { return libraryView.getSelectedPlaylist(); }
    
    //Used for cmd+f keyboard shortcut focusing on the searchbox
    SearchBox& getSearchBox() {return searchBox;}
    
    void playingPlaylist (bool isPlayingPlaylist);

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
    
    ValueTree currentlyPlayingList;
    String currentlyPlayingName;
    
};



#endif //H_GUICONTROL