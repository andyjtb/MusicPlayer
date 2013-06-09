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
            ScopedPointer<LastFmAuthPopup> authWindow;
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
                URL sessionAddress("http://ws.audioscrobbler.com/2.0/?method=auth.getSession&api_key="+apiKey+"&token="+apiToken+"&api_sig="+generateApiSig("auth.getSession"));
                
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
    
    if (method == "auth.getToken") {
        apiSigString = "api_key"+apiKey+"method"+method+"token"+apiToken+"5502dc6ec6a34709b17139cf6a0026b8";
    }
    else
        apiSigString = "api_key"+apiKey+"method"+method+"token"+apiToken+"5502dc6ec6a34709b17139cf6a0026b8";
        
    MD5 apiSig = MD5(apiSigString.toUTF8());
    
    return apiSig.toHexString();
}

void LastFmConnection::sendNowPlaying(ValueTree playingInfo)
{
//    URL nowPlayingAddress("http://ws.audioscrobbler.com/2.0/?method=track.updateNowPlaying&artist="+playingInfo.getProperty(MusicColumns::columnNames[MusicColumns::Artist]).toString()+"&api_key="+apiKey+"&token="+apiToken+"&api_sig="+apiSig.toHexString());
}