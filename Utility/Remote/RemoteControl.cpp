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

RemoteInterprocessConnection* RemoteControl::createConnectionObject()
{
    DBG("Creating connection");
    RemoteInterprocessConnection* newConnection = new RemoteInterprocessConnection ();
    
    //remoteConnections.add (newConnection);
    return newConnection;
}


