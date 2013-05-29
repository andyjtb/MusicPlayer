//
//  RemoteControl.cpp
//  MusicPlayer
//
//  Created by Andy on 28/12/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "RemoteControl.h"

RemoteControl::RemoteControl ()
{    
    beginWaitingForSocket(8888);
}

RemoteControl::~RemoteControl()
{
    remoteConnections.clear();
    stop();
}

RemoteInterprocessConnection* RemoteControl::createConnectionObject()
{
    DBG("Creating connection");
    RemoteInterprocessConnection* newConnection = new RemoteInterprocessConnection ();
    newConnection->setControls(guiControl, audioControl);
    
    remoteConnections.add (newConnection);
    return newConnection;
}

void RemoteControl::setControls (GuiControl* gui, AudioControl* audio)
{
    guiControl.set(gui, false);
    audioControl.set(audio, false);
}
