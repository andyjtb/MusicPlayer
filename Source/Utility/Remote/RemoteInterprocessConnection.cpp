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
        sendLibraryData();
    }
    
    if (stringMessage.startsWith("PlayTrack"))
    {
        int libID = stringMessage.fromFirstOccurrenceOf("PlayTrack ", false, true).getIntValue();
        DBG("Lib id = " << libID);
        ValueTree test (singletonLibraryTree.getChildWithProperty(MusicColumns::columnNames[MusicColumns::LibID], libID));
        if (test.isValid()) {
            tableSelectedRow = test;
            tableShouldPlay = true;
        }
        
    }
    if (stringMessage == "Play")
    {
        if (tablePlayingRow.isValid()) {
            if (!singletonPlayState.getValue())
                singletonPlayState = true;
        }
    }
    if (stringMessage.startsWith("Pause"))
    {
        if(singletonPlayState.getValue())
        {
            singletonPlayState = false;
        }
    }
    
    if (stringMessage.startsWith("Next")) {
        if (tableSelectedRow.isValid()) {
            int toPlay = filteredDataList.indexOf(tableSelectedRow);
            ++toPlay;
            if (toPlay < filteredDataList.getNumChildren())
            {
                tableSelectedRow = filteredDataList.getChild(toPlay);
                if (singletonPlayState.getValue())
                    tableShouldPlay = true;
                else
                    tableLoadSelected = true;
            }
        }
        
    }
    if (stringMessage.startsWith("Previous")) {
        if (tableSelectedRow.isValid()) {
            int toPlay = filteredDataList.indexOf(tableSelectedRow);
            --toPlay;
            if (toPlay >= 0)
            {
                tableSelectedRow = filteredDataList.getChild(toPlay);
                if (singletonPlayState.getValue())
                    tableShouldPlay = true;
                else
                    tableLoadSelected = true;
            }
        }
        
    }
    if (stringMessage.startsWith("Position: "))
    {
        guiControl->setPosition(stringMessage.fromFirstOccurrenceOf("Position: ", false, true).getDoubleValue());
    }
    if (stringMessage.startsWith("Volume: "))
    {
        guiControl->setVolume(stringMessage.fromFirstOccurrenceOf("Volume: ", false, true).getDoubleValue());
    }
}

void RemoteInterprocessConnection::sendString (String incomingString)
{
    MemoryBlock messageData (incomingString.toUTF8(), (size_t) incomingString.getNumBytesAsUTF8());
    sendMessage(messageData);
}

void RemoteInterprocessConnection::setControls(GuiControl *gui, AudioControl *audio)
{
    guiControl.set(gui, false);
    audioControl.set(audio, false);
}

void RemoteInterprocessConnection::sendPlayingData()
{
    sendString("Song: " + tablePlayingRow.getProperty(MusicColumns::columnNames[MusicColumns::Song]).toString());
    sendString("Artist: " + tablePlayingRow.getProperty(MusicColumns::columnNames[MusicColumns::Artist]).toString());
    //        sendString("TracksTotal: " + String(filteredDataList.getNumChildren()));
    //        sendString("TrackNum: " + String(filteredDataList.indexOf(tablePlayingRow)+1));
    sendTrackNums();
    sendLength(audioControl->getTransportLength());
    sendVolume(audioControl->getVolume());
    //        String currentAlbum = tablePlayingRow.getProperty(MusicColumns::columnNames[MusicColumns::Album]).toString();
    
    sendString("AlbumTitle: " + tablePlayingRow.getProperty(MusicColumns::columnNames[MusicColumns::Album]).toString());
    sendAlbumArt();
    
    sendString("NewTrack");
    sendString("PlayState: " + singletonPlayState.getValue().toString());  
}

void RemoteInterprocessConnection::sendAlbumArt()
{
    //Send album art memory block
    File audioFile = tablePlayingRow.getProperty(MusicColumns::columnNames[MusicColumns::Location]).toString();
    MemoryBlock artMemoryBlock;
    
    String type = TagReader::fileImageToMemoryBlock(audioFile, artMemoryBlock);
    sendString("AlbumArt: " + type);
    sendMessage(artMemoryBlock);
}

void RemoteInterprocessConnection::sendTrackNums()
{
    //int currentLibId = tablePlayingRow.getProperty(MusicColumns::columnNames[MusicColumns::LibID]);
    ValueTree searchTree = filteredDataList.getChildWithProperty(MusicColumns::columnNames[MusicColumns::LibID], tablePlayingRow.getProperty(MusicColumns::columnNames[MusicColumns::LibID]));
    
    if (searchTree.isValid()) {
        sendString("TracksTotal: " + String(filteredDataList.getNumChildren()));
        sendString("TrackNum: " + String(filteredDataList.indexOf(searchTree)+1));
    }   
}

void RemoteInterprocessConnection::sendLength(double length)
{
    for (int counter = 0; counter < remoteConnections.size(); counter++)
    {
        remoteConnections[counter]->sendString("Length: " + String(length));
    }
}
void RemoteInterprocessConnection::sendPosition (double position)
{
    for (int counter = 0; counter < remoteConnections.size(); counter++)
    {
        remoteConnections[counter]->sendString("Position: " + String(position));
    }
}
void RemoteInterprocessConnection::sendVolume (double volume)
{
    for (int counter = 0; counter < remoteConnections.size(); counter++)
    {
        remoteConnections[counter]->sendString("Volume: " + String(volume));
    }
}
void RemoteInterprocessConnection::sendPlayState()
{
    for (int counter = 0; counter < remoteConnections.size(); counter++)
    {
        remoteConnections[counter]->sendString("PlayState: " + singletonPlayState.getValue().toString());
    }
}

void RemoteInterprocessConnection::sendPlaylistData()
{
    sendString("Playlists");
    MemoryOutputStream playlistOutput;
    singletonPlaylistsTree.writeToStream(playlistOutput);
    
    MemoryBlock playlistMemoryBlock = playlistOutput.getMemoryBlock();
    
    sendMessage(playlistMemoryBlock);
}

void RemoteInterprocessConnection::sendLibraryData()
{
    sendString("Library");
    MemoryOutputStream libraryOutput;
    filteredDataList.writeToStream(libraryOutput);
    
    MemoryBlock libraryMemoryBlock = libraryOutput.getMemoryBlock();
    
    sendMessage(libraryMemoryBlock);
    
    sendPlaylistData();
}