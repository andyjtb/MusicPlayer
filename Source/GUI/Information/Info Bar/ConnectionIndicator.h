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

class ConnectionIndicator : public Component,
                            public TooltipClient
{
public:
    ConnectionIndicator();
    ~ConnectionIndicator();
    
    void paint (Graphics& g);
    void resized();
    
    void remoteConnectionChanged(String ipAddress);
    
    void timerCallback();
    
    String getTooltip();

private:    
    TooltipWindow tipWindow;
    StringArray connectedTo;
    bool connected;
    
    String tooltip;
    Array<IpAddress> ips;
};

#endif
