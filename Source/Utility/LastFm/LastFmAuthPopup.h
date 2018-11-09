//
//  LastFmAuthPopup.h
//  MusicPlayer
//
//  Created by Andy on 09/06/2013.
//
//

#ifndef LASTFMAUTHPOPUP
#define LASTFMAUTHPOPUP

#include "JuceHeader.h"

class LastFmAuthPopup  : public Component,
                         public Button::Listener
{
public:
    LastFmAuthPopup (String _apiKey, String _apiToken);
    ~LastFmAuthPopup();
    
    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    
    String getSessionKey () { return sessionKey; }
    String getUserName() { return userName; }
private:
    TextButton authorise;
    TextButton start;
    
    String apiKey, apiToken, sessionKey, userName;
};


#endif 
