/*
 *  SpeedPitch.cpp
 *  MusicPlayer
 *
 *  Created by Andy on 14/03/2013.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include "SpeedPitch.h"

SpeedPitch::SpeedPitch (AudioControl* incomingAudioControl)
{
    audioControl = incomingAudioControl;
    
    addAndMakeVisible (&speed);
    speed.setRange (0.5, 1.5, 0.01);
    speed.setSliderStyle (Slider::Rotary);
    speed.setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    speed.setValue(audioControl->getSoundTouchSettings().tempo);
    speed.addListener (this);

    addAndMakeVisible (&pitch);
    pitch.setRange (0.5, 1.5, 0.01);
    pitch.setSliderStyle (Slider::Rotary);
    pitch.setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    pitch.setValue(audioControl->getSoundTouchSettings().pitch);
    pitch.addListener (this);

    setSize (550, 275);
}

SpeedPitch::~SpeedPitch()
{
}

void SpeedPitch::paint (Graphics& g)
{
    g.fillAll (Colours::white);

    g.setColour (Colours::black);
    g.setFont (Font (23.2000f, Font::plain));
    g.drawText ("Speed",
                155, 45, 68, 30,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (23.2000f, Font::plain));
    g.drawText ("Pitch",
                305, 45, 68, 30,
                Justification::centred, true);
}

void SpeedPitch::resized()
{
    speed.setBounds (135, 80, 112, 120);
    pitch.setBounds (285, 80, 111, 120);
}

void SpeedPitch::sliderValueChanged (Slider* sliderThatWasMoved)
{
    if (sliderThatWasMoved == &speed)
    {
        audioControl->setPlaybackSpeed(speed.getValue());
    }
    else if (sliderThatWasMoved == &pitch)
    {
        audioControl->setPitch(pitch.getValue());
    }
}
