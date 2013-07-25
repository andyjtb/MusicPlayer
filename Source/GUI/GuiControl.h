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

#include "PlaybackControl.h"
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
The main control for all Gui interaction, contains all of the top level Gui components
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
    
    /** @internal */
    void paint(Graphics& g);
    /** @internal */
	void resized();
	
    /** Sets the pointer to the AudioControl class */
	void setAudioControl (AudioControl* incomingAudioControl);
    /** Returns a pointer to the AudioControl class */
	AudioControl* getAudioControl () { return audioControl; }
    /** Callback for the various ActionBroadcasters registered to this class - Library Import, library table selected row and transport position  */
	void actionListenerCallback (const String& message);
	/** Callback for the various Value variables registered to this class - PlayState, Art update, Table update and volume */
	void valueChanged (Value& valueChanged);
    /** Updates the transport bars position every 100ms and the output meters every 50ms */
	void timerCallback(int timerId);
	/** Updates the InfoBar, TrackInfo and AlbumArt when a different song is selected in the MusicLibraryTable */
	void updateSelectedDisplay ();
	/** Callback for a number of change listeners registered to this class - AudioControl (load file) and LibraryTreeView (different playlist selected)
     */
	void changeListenerCallback(ChangeBroadcaster* changeBroadcaster);
    /** Takes a value tree and loads the song it represents, updating the appropriate AlbumArt and TrackInfo displays
     @param treeToLoad ValueTree representing the file to load
     @param shouldPlay Whether the file should begin playing after being loaded
     */
    void loadFile(ValueTree treeToLoad, bool shouldPlay);
    
    //Track control
    /** Plays next song in currentlyPlayingList - Called by the PlaybackControl class, also called when each song ends */
    void next();
    /** Plays previous song in currentlyPlayingList - Does nothing if at the beginning of the list */
    void previous();
    
    //Remote
    /** Sets the volume and updates the volume control display - Called by the remote control */
    void setVolume(double incomingVolume);
    /** Sets the transport postion and updates the transport control display - Called by the remote control */
    void setPosition(double incomingPosition);
    
    //Text Editor Callbacks
    /** SearchBox change callback, searchs for the string entered in the search box, updates MusicLibraryTable.
     Also updates track number display on the remote
     */
    void textEditorTextChanged (TextEditor &textEditor);
    /** Returns the valuetree containing the currently playing data */
    ValueTree getCurrentlyPlayingList() { return currentlyPlayingList; }
    
    //Menu Bar function
    /** Shows the effects menu when selected in the top menu bar */
    void showEffectsMenu();
    /** Returns a pointer to the infoBar */
    InfoBar* getInfoBar() { return &infoBar; }
    /**
     Function called whenever a remote connection is made or lost, updates the infoBar connection display
     @param ipAddress The IP address that changed
     */
    void remoteConnectionChanged(String ipAddress) { infoBar.getConnectionIndicator().remoteConnectionChanged(ipAddress);}
    
    /**
     Sets the playlist, changing the information displayed in the MusicLibraryTable, called from the remoteInterprocessConnection
     @param incomingPlaylist The name of the playlist to display
     */
    void setPlaylist (String incomingPlaylist);
    /** Returns the currently selected playlist name as a string
     */
    String getPlaylist() { return libraryView.getSelectedPlaylist(); }
    
    /** Used for cmd+f keyboard shortcut to focus on the searchbox
     @return Reference to the SearchBox
     */
    SearchBox& getSearchBox() {return searchBox;}
    
    /**
     Changes the currentlyPlayingPlaylist and currentlyPlayingName based on whether the input is true or false
     @param isPlayingPlaylist Whether the application is playing the loaded file from a playlist or the general user library
     */
    void playingPlaylist (bool isPlayingPlaylist);

private:
	AudioControl* audioControl;
    
    PlaybackControl playbackControl;
	VolumeControl volumeControl;
	OutputMeter outputMeters;
	TransportSlider transport;
	
    TrackInfo trackInfo;
	AlbumArt albumArt;
    
    InfoBar infoBar;
    
    SearchBox searchBox;
    
    MusicLibraryDropTarget musicLibraryDropTarget;
    MusicLibraryTable* musicTable;

    LibraryTreeView libraryView;

//Effects pointers
    ScopedPointer<TabbedComponent> effectsTabbed;
    ScopedPointer<Equaliser> equaliser;
    ScopedPointer<SpeedPitch> speedPitch;
    
    ValueTree currentlyPlayingList;
    String currentlyPlayingName;
    
//Last Fm Scrobbling
    Time startedPlaying;
    double timePlaying;
    bool scrobbled;
    
};



#endif //H_GUICONTROL