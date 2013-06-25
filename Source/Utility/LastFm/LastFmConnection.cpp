//
//  LastFmConnection.cpp
//  MusicPlayer
//
//  Created by Andy on 09/06/2013.
//
//

#include "LastFmConnection.h"

LastFmConnection::LastFmConnection()
{
    enabled.addListener(this);
    apiKey = "5becb8a0259e2b0ee071e33c970610b4";
}

LastFmConnection::~LastFmConnection()
{}

bool LastFmConnection::getAuthToken()
{
    URL tokenAddress("http://ws.audioscrobbler.com/2.0/?method=auth.gettoken&api_key="+apiKey);
    
    ScopedPointer<XmlElement> token;
    token = tokenAddress.readEntireXmlStream();
    
    if (token != nullptr)
    {
        String status = token->getAttributeValue(0);
        if (status == "ok")
        {
            apiToken = token->getChildElement(0)->getAllSubText();
            
            return true;
        }
        else
        {
            //int statusInt = status.getIntValue();
            DBG("Status error " << status);
            return false;
        }
    }
    
    else
        return false;
}

void LastFmConnection::connect()
{
    //Connect
    if (sessionKey == String::empty)
    {
        if (getAuthToken())
        {
            //Display auth dialog
            LastFmAuthPopup* authWindow;
            authWindow = new LastFmAuthPopup(apiKey, apiToken);
            
            DialogWindow::LaunchOptions o;
            
            o.content.setOwned (authWindow);
            o.content->setSize (authWindow->getWidth(), authWindow->getHeight());
            
            o.dialogTitle                   = "Please Authorise MusicPlayer";
            o.dialogBackgroundColour        = Colours::lightgrey;
            o.escapeKeyTriggersCloseButton  = true;
            o.useNativeTitleBar             = false;
            o.resizable                     = false;
            
            if (o.runModal() != 0 )
            {
                String apiSig = generateApiSig("auth.getSession");
                
                URL sessionAddress("http://ws.audioscrobbler.com/2.0/?method=auth.getSession&api_key="+apiKey+"&token="+apiToken+"&api_sig="+apiSig);
                
                ScopedPointer<XmlElement> session;
                session = sessionAddress.readEntireXmlStream();
                
                if (session != nullptr)
                {
                    String status = session->getAttributeValue(0);
                    
                    if (status == "ok")
                    {
                        ScopedPointer<XmlElement> info;
                        info = session->getChildElement(0);
                        
                        userName = info->getChildElement(0)->getAllSubText();
                        sessionKey = info->getChildElement(1)->getAllSubText();
                        
                        DBG(userName)
                        
                        connected = true;
                        lastFmButton->repaint();
                    }
                    else
                    {
                        DBG("Status error " << status);
                    }
                }
            }
        }
    }
    else
    {
        //Session key exists
        connected = true;
        lastFmButton->repaint();
    }
}

void LastFmConnection::valueChanged(Value &changed)
{
    if (changed == enabled)
    {
        if (enabled.getValue())
        {
            connect();
        }
        else
        {
            //disconnect or something....
            connected = false;
        }
        lastFmButton->repaint();
    }
}

String LastFmConnection::generateApiSig(String method)
{
    String apiSigString;

    apiSigString = "api_key"+apiKey+"method"+method+"token"+apiToken+"5502dc6ec6a34709b17139cf6a0026b8";
    
    MD5 apiSig = MD5(apiSigString.toUTF8());
    
    return apiSig.toHexString();
}

String LastFmConnection::lastFmString (ValueTree playingInfo, Identifier infoRequired)
{
    return URL::addEscapeChars(playingInfo.getProperty(infoRequired).toString(), true);
}

void LastFmConnection::sendNowPlaying(ValueTree playingInfo)
{
    Identifier artist = MusicColumns::columnNames[MusicColumns::Artist];
    Identifier album = MusicColumns::columnNames[MusicColumns::Album];
    Identifier track = MusicColumns::columnNames[MusicColumns::Song];
    
    String apiSigString = "album"+playingInfo.getProperty(album).toString()
    +"api_key"+apiKey
    +"artist"+playingInfo.getProperty(artist).toString()
    +"method"+"track.updateNowPlaying"
    +"sk"+sessionKey
    +"track"+playingInfo.getProperty(track).toString()
    +"5502dc6ec6a34709b17139cf6a0026b8";

    MD5 apiSig = MD5(apiSigString.toUTF8());
    
    URL nowPlayingAddress("http://ws.audioscrobbler.com/2.0/?method=track.updateNowPlaying&artist="+lastFmString(playingInfo, artist)
            +"&album="+lastFmString(playingInfo, album)
            +"&track="+lastFmString(playingInfo, track)
            +"&api_key="+apiKey
            +"&sk="+sessionKey
            +"&api_sig="+apiSig.toHexString());
    
    ScopedPointer<XmlElement> response;
    response = nowPlayingAddress.readEntireXmlStream(true);
    
    saveXmlTest(response);
    
    if (response != nullptr)
    {
        String status = response->getAttributeValue(0);
        
        DBG("Response" + response->getAllSubText());
        
        if (status == "ok")
        {
            DBG(status);
        }
    }
}