//
//  TrackDialog.h
//  MusicPlayer
//
//  Created by Andy on 06/12/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef TRACKDIALOG 
#define TRACKDIALOG

#include "../JuceLibraryCode/JuceHeader.h"
#include "Settings.h"
#include "MusicLibraryHelpers.h"
#include "TrackTabbed.h"
/** The dialog window containing the TrackTabbed class, provides the next, previous and ok buttons */
class TrackDialog  :	public Component,
                    public ButtonListener
{
public:
    /** Constructor - Takes the index of the currently selected track  */
    TrackDialog (int incomingTrack);
    /** Destructor */
    ~TrackDialog();
	
    /** @internal */
    void paint (Graphics& g);
    /** @internal */
    void resized();
    /** Callback when the previous, next or ok buttons are clicked */
    void buttonClicked (Button* buttonClicked);
    /** Loads each of the displays in the tabbed component with the information for the new track - Incremented or decremented by the buttonClicked function */
    void setTrack();
private:
    TrackTabbed trackTabbed;
    
    TextButton ok;
    TextButton next; 
    TextButton previous;
    
    //ValueTree filteredDataList;
    int selectedTrack;
};


#endif
