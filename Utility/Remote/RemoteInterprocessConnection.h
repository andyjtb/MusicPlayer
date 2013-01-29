//
//  RemoteInterprocessConnection.h
//  MusicPlayer
//
//  Created by Andy on 28/12/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef REMOTEINTERPROCESSCONNECTION
#define REMOTEINTERPROCESSCONNECTION 

#include "../JuceLibraryCode/JuceHeader.h"
#include "Settings.h"
#include "MusicLibraryHelpers.h"
#include "GuiControl.h"
#include "AudioControl.h"

class RemoteInterprocessConnection  : public InterprocessConnection
{
public:
    RemoteInterprocessConnection ();
    ~RemoteInterprocessConnection();
    
    void connectionMade();
    void connectionLost();
    void messageReceived (const MemoryBlock& message);
    void sendString(String incomingString);
    
    void setControls(GuiControl* gui, AudioControl* audio);
    
    void sendPlayingData();
    void sendAlbumArt();
    void sendLength (double length);
    void sendPosition (double position);
    void sendVolume (double volume);
    void sendPlayState();

private:
    int connectionNumber;
    
    OptionalScopedPointer<GuiControl> guiControl;
    OptionalScopedPointer<AudioControl> audioControl;

    MemoryBlock artMemoryBlock;
};


#endif
