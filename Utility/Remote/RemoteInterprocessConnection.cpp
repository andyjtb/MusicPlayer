//
//  RemoteInterprocessConnection.cpp
//  MusicPlayer
//
//  Created by Andy on 28/12/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "RemoteInterprocessConnection.h"

RemoteInterprocessConnection::RemoteInterprocessConnection () : InterprocessConnection(true)
{
    remoteNumConnections++;
}

RemoteInterprocessConnection::~RemoteInterprocessConnection()
{
    
}

void RemoteInterprocessConnection::connectionMade()
{
    DBG("Connection #" + String (remoteNumConnections) + " - connection started");
    DBG("Connected to = " << getConnectedHostName());
    
    String chickens ("cheese");
    MemoryBlock messageData (chickens.toUTF8(), (size_t) chickens.getNumBytesAsUTF8());
    sendMessage(messageData);
}

void RemoteInterprocessConnection::connectionLost()
{
    DBG("Connection #" + String (remoteNumConnections) + " - connection lost");
    remoteNumConnections--;
}

void RemoteInterprocessConnection::messageReceived (const MemoryBlock& message)
{
    String stringMessage = message.toString();
    DBG("Connection #" + String (remoteNumConnections) + " - message received: " + stringMessage);
    
    if (stringMessage.startsWith("Play"))
        {
            ValueTree test (singletonLibraryTree.getChildWithProperty(MusicColumns::columnNames[MusicColumns::Song], stringMessage.fromFirstOccurrenceOf("Play ", false, true)));
            if (test.isValid()) {
                tableSelectedRow = test;
                tableShouldPlay = true;
            }
            
        }
    if (stringMessage.startsWith("Next")) {
        if (tableSelectedRow.isValid()) {
            int toPlay = filteredDataList.indexOf(tableSelectedRow);
            toPlay++;
            tableSelectedRow = filteredDataList.getChild(toPlay);
            tableShouldPlay = true;
        }
        
    }
    if (stringMessage.startsWith("Previous")) {
        if (tableSelectedRow.isValid()) {
            int toPlay = filteredDataList.indexOf(tableSelectedRow);
            toPlay--;
            tableSelectedRow = filteredDataList.getChild(toPlay);
            tableShouldPlay = true;
        }
        
    }
}