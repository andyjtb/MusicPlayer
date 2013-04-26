//
//  RemoteInterprocessConnection.h
//  MusicPlayer
//
//  Created by Andy on 28/12/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef REMOTEINTERPROCESSCONNECTION
#define REMOTEINTERPROCESSCONNECTION 

#include "../JuceLibraryCode/JuceHeader.h"
#include "Settings.h"
#include "MusicLibraryHelpers.h"
#include "GuiControl.h"
#include "AudioControl.h"

/** An object which controls a single connection to one mobile remote control device
 @see RemoteControl
 */
class RemoteInterprocessConnection  : public InterprocessConnection
{
public:
    /** Constructor
     */
    RemoteInterprocessConnection ();
    /** Destructor
     */
    ~RemoteInterprocessConnection();
    
    /** Called when the connection to the remote device is made
     */
    void connectionMade();
    /** Called when the connection to the remote device is lost
     */
    void connectionLost();
    /** Called whenever a message is recieved from the remote device, triggers necessary changes depending on the message from the remote device
     */
    void messageReceived (const MemoryBlock& message);
    /** Converts a string to a MemoryBlock and sends it
     @param incomingString String that is send
     */
    void sendString(String incomingString);
    
    /** Sets pointers to GUI and Audio controls, allowing the remote connection to gain information to be sent and also cause things to happen depending on signals from the remote device
     @param gui Pointer to the GuiControl class
     @param audio Pointer to the AudioControl class
     */
    void setControls(GuiControl* gui, AudioControl* audio);
    
    /** Sends data about the currently playing song to the remote device, tile, album, artist, album art, length etc.
     */
    void sendPlayingData();
    /** Sends the album art for the currently playing song. First sends a message to the remote device telling it expect album art then sends the album art data
     */
    void sendAlbumArt();
    /** Sends the number of tracks in the current playlist and the number of the song that is currently playing
     */
    void sendTrackNums();
    /** Sends the current playback position
     */
    void sendPosition (double position);
    /** Sends the current volume
     */
    void sendVolume (double volume);
    /** Sends whether the application is playing audio or not
     */
    void sendPlayState();
    /** Sends the library and playlist ValueTrees as MemoryBlocks of binary data
     */
    void sendLibraryData();
    
private:
    //Called as part of send library data
    /** @internal
     */
    void sendPlaylistData();
    
    int connectionNumber;
    String connectionName;
    String lastAlbum;
    
    OptionalScopedPointer<GuiControl> guiControl;
    OptionalScopedPointer<AudioControl> audioControl;

    //MemoryBlock artMemoryBlock;
};


#endif
