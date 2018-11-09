/*
 *  InfoBar.h
 *  MusicPlayer
 *
 *  Created by Andy on 26/10/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INFOBAR
#define INFOBAR

#include "JuceHeader.h"
#include "Settings/Settings.h"
#include "ConnectionIndicator.h"
#include "MetaData/TagReader.h"
#include "LastFmButton.h"

/** The InfoBar at the bottom of the interface */
class InfoBar  : public Component,
                 public ValueTree::Listener
{
public:
    /** Constructor */
    InfoBar ();
    /** Destructor */
    ~InfoBar();

    /** @internal */
    void paint (Graphics& g) override;
    /** @internal */
    void resized() override;
    /** Updates the information displayed in the bar - Library size (file size and number of files) and length */
    void updateBar(); 
    
    /** ValueTree callbacks */
    void valueTreePropertyChanged (ValueTree &treeWhosePropertyHasChanged, const Identifier &property) override {}
    void valueTreeChildAdded (ValueTree &parentTree, ValueTree &childWhichHasBeenAdded) override {}
    void valueTreeChildRemoved (ValueTree &parentTree, ValueTree &childWhichHasBeenRemoved, int) override {}
    /** Updates info bar - Everytime the table changes it re-orders/re-creates the filtered data list, including sorting it so this is the most called callback */ 
    void valueTreeChildOrderChanged (ValueTree &parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex) override;
    void valueTreeParentChanged (ValueTree &treeWhoseParentHasChanged) override {}
    void valueTreeRedirected (ValueTree &treeWhichHasBeenChanged) override {}
    
    /** Returns the connection indicator component */
    ConnectionIndicator& getConnectionIndicator () { return indicator;}
    /** Displays a message if a file can't be read or found */
    void displayFileStatus (File& file, int result);
    /** Allows the user to reload a file that can't be found if they double click the "can't be found" message */
    void mouseDoubleClick (const MouseEvent& e) override;
    
private:
    String size, time, numTracks;
    ConnectionIndicator indicator;
    LastFmButton lastFm;
    Label infoLabel;
    
    File currentFile;
    bool notFound, notRead, loading;
};
#endif
