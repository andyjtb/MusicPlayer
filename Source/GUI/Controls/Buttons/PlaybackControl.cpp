//
//  PlaybackControl.cpp
//  MusicPlayer
//
//  Created by Andy on 29/04/2013.
//
//

#include "PlaybackControl.h"

#include "GuiControl.h"

PlaybackControl::PlaybackControl(GuiControl* _parent)
{
    addAndMakeVisible(&playButton);
    
    addAndMakeVisible(&forwardButton);
    forwardButton.addListener(this);
    
    addAndMakeVisible(&backButton);
    backButton.addListener(this);
    
    parent.set(_parent, false);
}

PlaybackControl::~PlaybackControl()
{
    
}

void PlaybackControl::paint(Graphics &g)
{
}

void PlaybackControl::resized()
{
    backButton.setBounds(0, 10, getHeight()-15, getHeight()-15);
    playButton.setBounds(55, 0, getHeight(), getHeight());
    forwardButton.setBounds(125, 10, getHeight()-15, getHeight()-15);
}

void PlaybackControl::buttonClicked(Button *clicked)
{
    if (clicked == &forwardButton) {
        parent->next();
    }
    else if (clicked == &backButton)
    {
        parent->previous();
    }
}