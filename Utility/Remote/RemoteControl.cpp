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
    stop();
    remoteConnections.clear();
}

InterprocessConnection* RemoteControl::createConnectionObject()
{
    DBG("Creating connection");
    InterprocessConnection* newConnection = new RemoteInterprocessConnection ();
    
    remoteConnections.add (newConnection);
    return newConnection;
}


