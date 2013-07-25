//
//  LastFmAuthPopup.h
//  MusicPlayer
//
//  Created by Andy on 09/06/2013.
//
//

#ifndef LASTFMAUTHPOPUP
#define LASTFMAUTHPOPUP

#include "../JuceLibraryCode/JuceHeader.h"

class LastFmAuthPopup  : public Component,
                         public ButtonListener
{
public:
    LastFmAuthPopup (String _apiKey, String _apiToken);
    ~LastFmAuthPopup();
    
    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
    
    String getSessionKey () { return sessionKey; }
    String getUserName() { return userName; }
private:
    TextButton authorise;
    TextButton start;
    
    String apiKey, apiToken, sessionKey, userName;
};


#endif 
