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
#include "ConnectionIndicatorBinary.h"

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
    Image on, off;
    TooltipWindow tipWindow;
    StringArray connectedTo;
    bool connected;
};

#endif
