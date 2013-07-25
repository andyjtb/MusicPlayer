//
//  LastFmConnection.h
//  MusicPlayer
//
//  Created by Andy on 09/06/2013.
//
//

#ifndef LASTFMCONNECTION
#define LASTFMCONNECTION

#include "../JuceLibraryCode/JuceHeader.h"
#include "LastFmAuthPopup.h"
#include "MusicLibraryHelpers.h"


class LastFmConnection : public ValueListener
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
    
    void valueChanged (Value& changed);
    
    void setLastFmButton(Component* button);
    
    Value getEnabled () { return enabled; }
    String& getSessionKey() { return sessionKey;}
    String& getUserName() { return userName; }
    
    String lastFmString (ValueTree playingInfo, Identifier infoRequired);
    
    void saveXmlTest (XmlElement* element) { File testFile (File::getSpecialLocation(File::userDesktopDirectory).getFullPathName()+ "/LastFmTest"); element->writeToFile(testFile, String::empty);}
    
    void displayError (XmlElement* error);
    
private:
    bool connected;
    
    String apiKey, apiToken, apiSession, sessionKey, userName;
    
    Value enabled;
    
    ValueTree toScrobble;
    File scrobblesFile;
};

#endif
