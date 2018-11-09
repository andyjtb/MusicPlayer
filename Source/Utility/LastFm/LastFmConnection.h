//
//  LastFmConnection.h
//  MusicPlayer
//
//  Created by Andy on 09/06/2013.
//
//

#ifndef LASTFMCONNECTION
#define LASTFMCONNECTION

#include "JuceHeader.h"
#include "LastFmAuthPopup.h"
#include "iTunesLibrary/MusicLibraryHelpers.h"


class LastFmConnection : public Value::Listener
{
public:
    LastFmConnection();
    ~LastFmConnection();
    
    bool isConnected() { return connected; }
    bool getAuthToken();
    void connect();
    
    void sendNowPlaying (ValueTree playingInfo);
    void getTrackInfo (ValueTree selectedTrack);
    void scrobbleTrack (ValueTree incomingTrack, Time startedPlaying);
    
    String generateApiSig (String method);
    
    void valueChanged (Value& changed) override;
    
    void setLastFmButton(Component* button);
    
    Value getEnabled () { return enabled; }
    String& getSessionKey() { return sessionKey;}
    String& getUserName() { return userName; }
    
    String lastFmString (ValueTree playingInfo, Identifier infoRequired);
    
    void saveXmlTest (XmlElement* element) { File testFile (File::getSpecialLocation(File::userDesktopDirectory).getFullPathName()+ "/LastFmTest"); element->writeToFile(testFile, String());}
    
    void displayError (XmlElement* error);
    
private:
    bool connected;
    
    String apiKey, apiToken, apiSession, sessionKey, userName;
    
    Value enabled;
    
    ValueTree toScrobble;
    File scrobblesFile;
};

#endif
