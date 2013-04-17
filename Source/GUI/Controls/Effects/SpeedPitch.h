/*
 *  SpeedPitch.h
 *  MusicPlayer
 *
 *  Created by Andy on 14/03/2013.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SPEEDPITCH
#define SPEEDPITCH

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioControl.h"

class SpeedPitch  : public Component,
                    public SliderListener
{
public:
    SpeedPitch (AudioControl* incomingAudioControl);
    ~SpeedPitch();
    
    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider* sliderThatWasMoved);

private:
    Slider speed;
    Slider pitch;
    
    AudioControl* audioControl;
};


#endif 