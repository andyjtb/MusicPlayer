//
//  ConnectionIndicator.h
//  MusicPlayer
//
//  Created by Andy on 01/04/2013.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef CONNECTIONINDICATOR
#define CONNECTIONINDICATOR

#include "JuceHeader.h"
#include "Remote/J_IPAddress.h"
#include "Settings/Settings.h"

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
    void paint (Graphics& g) override;
    /** @internal */
    void resized() override;
    
    /** Called when a connection is made or lost, updates the list of connectioned contained in this class */
    void remoteConnectionChanged(String ipAddress);
    /** Displays the tooltip containing the local IP address and the connected devices */
    String getTooltip() override;

private:    
    TooltipWindow tipWindow;
    StringArray connectedTo;
    bool connected;
    
    String tooltip;
    Array<IpAddress> ips;
};

#endif
