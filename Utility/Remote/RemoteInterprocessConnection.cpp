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
    connectionNumber = remoteConnections.size();
}

RemoteInterprocessConnection::~RemoteInterprocessConnection()
{
    
}

void RemoteInterprocessConnection::connectionMade()
{
    DBG("Connection #" + String (connectionNumber) + " - connection started");
    DBG("Connected to = " << getConnectedHostName());
}

void RemoteInterprocessConnection::connectionLost()
{
    DBG("Connection #" + String (connectionNumber) + " - connection lost");
    remoteConnections.remove(connectionNumber);
}

void RemoteInterprocessConnection::messageReceived (const MemoryBlock& message)
{
    String stringMessage = message.toString();
    DBG("Connection #" + String (connectionNumber) + " - message received: " + stringMessage);
    
    if (stringMessage.startsWith("ConnectionMade"))
        {
            sendPlayingData();
        }
    
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
    if (stringMessage.startsWith("Position: "))
    {
        
    }
}

void RemoteInterprocessConnection::sendString (String incomingString)
{
    MemoryBlock messageData (incomingString.toUTF8(), (size_t) incomingString.getNumBytesAsUTF8());
    sendMessage(messageData);
}

void RemoteInterprocessConnection::sendPlayingData()
{
    sendString("Artist: " + tablePlayingRow.getProperty(MusicColumns::columnNames[MusicColumns::Artist]).toString());
    sendString("Song: " + tablePlayingRow.getProperty(MusicColumns::columnNames[MusicColumns::Song]).toString());
    sendString("AlbumTitle: " + tablePlayingRow.getProperty(MusicColumns::columnNames[MusicColumns::Album]).toString());
    sendString("TracksTotal: " + filteredDataList.getNumChildren());
    sendString("TrackNum: " + filteredDataList.indexOf(tablePlayingRow));
    sendString("PlayState: " + singletonPlayState.getValue().toString());            
    
    sendAlbumArt();
}

void RemoteInterprocessConnection::sendAlbumArt()
{
    sendString("AlbumArt");
    //Send album art memory block
}
void RemoteInterprocessConnection::sendLength(int length)
{
    for (int counter = 0; counter < remoteConnections.size(); counter++)
    {}
        //remoteConnections[counter].sendString("Length: " + length);
}
void RemoteInterprocessConnection::sendPosition (int position)
{
    for (int counter = 0; counter < remoteConnections.size(); counter++)
    {}
        //remoteConnections[counter].sendString("Position: " + position);
}
void RemoteInterprocessConnection::sendVolume (float volume)
{
    for (int counter = 0; counter < remoteConnections.size(); counter++)
    {}
    //remoteConnections[counter].sendString("Volume: " + String(volume));
}
void RemoteInterprocessConnection::sendPlayState()
{
//    for (int counter = 0; counter < remoteConnections.size(); counter++)
//    {}
//        remoteConnections[counter].
        sendString("PlayState: " + singletonPlayState.getValue().toString() );
}