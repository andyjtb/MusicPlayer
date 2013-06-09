//
//  LastFmAuthPopup.cpp
//  MusicPlayer
//
//  Created by Andy on 09/06/2013.
//
//

#include "LastFmAuthPopup.h"

LastFmAuthPopup::LastFmAuthPopup (String _apiKey, String _apiToken)
{
    apiKey = _apiKey;
    apiToken = _apiToken;
    
    addAndMakeVisible (&authorise);
    authorise.setButtonText ("Authorise");
    authorise.addListener (this);
    
    addAndMakeVisible (&start);
    start.setButtonText ("Start Last.FM");
    start.addListener (this);
    start.setEnabled(false);

    setSize (400, 200);
}

LastFmAuthPopup::~LastFmAuthPopup()
{
}

//==============================================================================
void LastFmAuthPopup::paint (Graphics& g)
{
    g.setColour (Colours::black);
    g.setFont (Font (15.0000f, Font::plain));
    g.drawText ("Clicking the authorise button will open your Web Browser.",
                20, 4, 368, 30,
                Justification::centred, true);
    
    g.setColour (Colours::black);
    g.setFont (Font (15.0000f, Font::plain));
    g.drawText ("Please allow MusicPlayer",
                20, 28, 368, 30,
                Justification::centred, true);
    
    g.setColour (Colours::black);
    g.setFont (Font (15.0000f, Font::plain));
    g.drawText ("Once you have allowed MusicPlayer, you my press Start",
                4, 108, 384, 30,
                Justification::centred, true);
}

void LastFmAuthPopup::resized()
{
    authorise.setBounds (128, 64, 150, 24);
    start.setBounds (128, 152, 150, 24);
}

void LastFmAuthPopup::buttonClicked (Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == &authorise)
    {
        URL userAuth ("http://www.last.fm/api/auth/?api_key="+apiKey+"&token="+apiToken);
        userAuth.launchInDefaultBrowser();
        
        start.setEnabled(true);
    }
    
    else if (buttonThatWasClicked == &start)
    {
        DialogWindow* dw = findParentComponentOfClass<DialogWindow>();
        if (dw != nullptr)
            dw->exitModalState (1);

    }
}