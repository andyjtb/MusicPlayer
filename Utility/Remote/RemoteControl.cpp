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
    remoteNumConnections = 0;
    beginWaitingForSocket(8888);
}

RemoteControl::~RemoteControl()
{
    stop();
    activeConnections.clear();
}

InterprocessConnection* RemoteControl::createConnectionObject()
{
    DBG("Creating connection");
    InterprocessConnection* newConnection = new RemoteInterprocessConnection ();
    
    activeConnections.add (newConnection);
    return newConnection;
}


