//
//  ConnectionIndicator.h
//  MusicPlayer
//
//  Created by Andy on 01/04/2013.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef CONNECTIONINDICATOR
#define CONNECTIONINDICATOR

#include "../JuceLibraryCode/JuceHeader.h"
#include "J_IPAddress.h"
#include "Settings.h"

/** A small green circle which becomes brighter when a remote control is connected */
class ConnectionIndicator : public Component,
                            public TooltipClient
{
public:
    /** Constructor */
    ConnectionIndicator();
    /** Destructor */
    ~ConnectionIndicator();
    
    /** @internal */
    void paint (Graphics& g);
    /** @internal */
    void resized();
    
    /** Called when a connection is made or lost, updates the list of connectioned contained in this class */
    void remoteConnectionChanged(String ipAddress);
    /** Displays the tooltip containing the local IP address and the connected devices */
    String getTooltip();

private:    
    TooltipWindow tipWindow;
    StringArray connectedTo;
    bool connected;
    
    String tooltip;
    Array<IpAddress> ips;
};

#endif
