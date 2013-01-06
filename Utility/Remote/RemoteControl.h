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

class RemoteControl :  public InterprocessConnectionServer
{
public:
        RemoteControl();
        ~RemoteControl();
 
private:
    RemoteInterprocessConnection* createConnectionObject();
    
    //OwnedArray <InterprocessConnection, CriticalSection> activeConnections;

};

#endif //REMOTECONTROL
