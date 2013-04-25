//
//  RemoteControl.h
//  MusicPlayer
//
//  Created by Andy on 28/12/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef REMOTECONTROL 
#define REMOTECONTROL

#include "../JuceLibraryCode/JuceHeader.h"
#include "RemoteInterprocessConnection.h"
#include "Settings.h"
#include "GuiControl.h"
#include "AudioControl.h"
/** A network connection server, listens in the background for activity on port 8888, creating a new connection object when activity is registered.
    All connection objects are added to the remoteConnections array stored in the Settings class
    @see RemoteInterprocessConnection, Settings
 */
class RemoteControl :  public InterprocessConnectionServer
{
public:
    RemoteControl();
    ~RemoteControl();
    
    /** Provide the server with pointers to the GUI and Audio controls, so that the pointer may be passed to each object that is created
     @param gui Pointer to the GuiControl class
     @param audio Pointer to the AudioControl class
     */
    void setControls (GuiControl* gui, AudioControl* audio);
    
private:
    RemoteInterprocessConnection* createConnectionObject();
    
    OptionalScopedPointer<GuiControl> guiControl;
    OptionalScopedPointer<AudioControl> audioControl;
    
};

#endif //REMOTECONTROL
