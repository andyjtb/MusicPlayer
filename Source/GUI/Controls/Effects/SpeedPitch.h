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

#include "JuceHeader.h"
#include "AudioControl.h"

/**
 A Component containing the speed and pitch sliders, when a slider is moved this class alerts the AudioControl
 */
class SpeedPitch  : public Component,
                    public Slider::Listener
{
public:
    /** Constructor - Passes a pointer to the AudioControl class, allowing this class to directly change the SoundTouch settings
     @see SoundTouch
     */
    SpeedPitch (AudioControl* incomingAudioControl);
    /** Destructor
     */
    ~SpeedPitch();
    
    /** @internal
     */
    void paint (Graphics& g) override;
    /** @internal
     */
    void resized() override;
    /** Calls when either slider moves, changes the SoundTouch settings based on the slider values
     */
    void sliderValueChanged (Slider* sliderThatWasMoved) override;

private:
    Slider speed;
    Slider pitch;
    
    AudioControl* audioControl;
};


#endif 
