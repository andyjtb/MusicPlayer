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
    String generateApiSig (String method);
    
    void valueChanged (Value& changed);
    
    void setLastFmButton(Component* button) { lastFmButton.set(button, false); }
    
    Value getEnabled () { return enabled; }
    String& getSessionKey() { return sessionKey;}
    String& getUserName() { return userName; }
    
private:
    bool connected;
    OptionalScopedPointer<Component> lastFmButton;
    
    String apiKey, apiToken, apiSession, sessionKey, userName;
    
    Value enabled;
};

#endif
