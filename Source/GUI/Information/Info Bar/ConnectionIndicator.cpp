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
    
    tipWindow.setMillisecondsBeforeTipAppears(300);
}

ConnectionIndicator::~ConnectionIndicator()
{}

void ConnectionIndicator::paint(Graphics &g)
{
    if (connected)
        g.drawImage (on,
                 0, 0, on.getWidth(), on.getHeight(),
                 0, 0, on.getWidth(), on.getHeight());
    else
        g.drawImage (off,
                     0, 0, off.getWidth(), off.getHeight(),
                     0, 0, off.getWidth(), off.getHeight());
}

void ConnectionIndicator::resized()
{
    on = ImageCache::getFromMemory (ConnectionIndicatorBinary::on_png, ConnectionIndicatorBinary::on_pngSize).rescaled(getWidth(), getHeight());
    
    off = ImageCache::getFromMemory (ConnectionIndicatorBinary::off_png, ConnectionIndicatorBinary::off_pngSize).rescaled(getWidth(), getHeight());
}

void ConnectionIndicator::remoteConnectionChanged(String ipAddress)
{
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
    
    repaint();
}

String ConnectionIndicator::getTooltip()
{
    String tooltip;
    Array<IpAddress> ips;
    IpAddress::findAllIpAddresses(ips);
    
    tooltip << "Your Ip: \n" << ips[1].toString();
    tooltip << "\n\nCurrently connected to: \n";
    
    for (int i = 0; i < remoteConnections.size(); i++)
    {
        tooltip << connectedTo[i] << "\n";
    }
    
    return tooltip;
}
