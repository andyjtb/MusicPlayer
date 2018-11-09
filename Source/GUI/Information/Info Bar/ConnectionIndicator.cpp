//
//  ConnectionIndicator.cpp
//  MusicPlayer
//
//  Created by Andy on 01/04/2013.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "ConnectionIndicator.h"

ConnectionIndicator::ConnectionIndicator()
{
    if (remoteConnections.size() > 0) 
        connected = true;
    else
        connected = false;
    
    IpAddress::findAllIpAddresses(ips);
    tooltip << "Your Ip: \n" << ips[1].toString();
    
    tipWindow.setMillisecondsBeforeTipAppears(300);
    
    
}

ConnectionIndicator::~ConnectionIndicator()
{}

void ConnectionIndicator::paint(Graphics &g)
{
    //Makes everything a variable amount smaller, so the edges don't look quite so squashed
    int edgeBuffer = 1;
    
    if (connected)
    {
        g.setColour (Colour (0xff27ff00));
        g.fillEllipse (0, 0, getWidth() - edgeBuffer, getHeight()- edgeBuffer);
        
        g.setColour (Colour (0xff686868));
        g.drawEllipse (0, 0, getWidth()- edgeBuffer, getHeight()- edgeBuffer, 1);
        
        g.setGradientFill (ColourGradient (Colour (0xbaffffff),
                                           2, 2,
                                           Colour (0x8000),
                                           getWidth()/2, getHeight()/2,
                                           true));
        g.fillRect (0, 0, getWidth()- edgeBuffer, getHeight()- edgeBuffer);
    }
    else
    {
        g.setColour (Colour (0xff4d9025));
        g.fillEllipse (0, 0, getWidth()- edgeBuffer, getHeight()- edgeBuffer);
        
        g.setColour (Colour (0xff686868));
        g.drawEllipse (0, 0, getWidth()- edgeBuffer, getHeight()- edgeBuffer, 1);
    }

}

void ConnectionIndicator::resized()
{

}

void ConnectionIndicator::remoteConnectionChanged(String ipAddress)
{
    DBG("Ip change = " << ipAddress);
    
    bool contains = connectedTo.contains(ipAddress);
    if (contains) {
        //If already exists then must be deleting
        connectedTo.removeString(ipAddress);
    }
    else
        connectedTo.add(ipAddress);
        
    if (remoteConnections.size() > 0) 
        connected = true;
    else
        connected = false;
    
    //Clear tooltip and re-make it whenever the remoteconnection changes
    tooltip = String();
    tooltip << "Your Ip: \n" << ips[1].toString();
    //If any connections exist
    if (remoteConnections.size() > 0)
    {
        tooltip << "\n\nCurrently connected to: \n";
        
        for (int i = 0; i < remoteConnections.size(); i++)
        {
            tooltip << connectedTo[i] << "\n";
        }
    }
    
    repaint();
}

String ConnectionIndicator::getTooltip()
{
    return tooltip;
}
