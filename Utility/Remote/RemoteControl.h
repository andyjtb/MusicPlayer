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

class RemoteControl :  public InterprocessConnectionServer
{
public:
        RemoteControl();
        ~RemoteControl();
 
    void setControls (GuiControl* gui, AudioControl* audio);
    
private:
    RemoteInterprocessConnection* createConnectionObject();
    
    OptionalScopedPointer<GuiControl> guiControl;
    OptionalScopedPointer<AudioControl> audioControl;
    
    //OwnedArray <InterprocessConnection, CriticalSection> activeConnections;

};

#endif //REMOTECONTROL
