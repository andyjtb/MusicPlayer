//
//  PlaybackControl.h
//  MusicPlayer
//
//  Created by Andy on 29/04/2013.
//
//

#ifndef __MusicPlayer__PlaybackControl__
#define __MusicPlayer__PlaybackControl__

#include "JuceHeader.h"
#include "PlayButton.h"
#include "ForwardButton.h"
#include "BackwardsButton.h"

class GuiControl;

/**
 A container class for the 3 playback buttons, play/pause, forward and backward
 
 @see PlayButton, ForwardButton, BackwardsButton,
 */
class PlaybackControl : public Component,
                        public Button::Listener
{
public:
	//==============================================================================
    /**
     Constructor - Takes a pointer to the parent GuiControl class to allow button clicks to call GuiControl functions
     */
	PlaybackControl(GuiControl* _parent);
    
    /**
     Destructor
     */
	~PlaybackControl();
    
    /** @internal */
    void paint(Graphics& g) override;
    /** @internal */
	void resized() override;
    
    /** 
     Called if previous or forward buttons are pressed, calls corresponding functions in GuiControl
     @param clicked The button that was clicked
     @see GuiControl
     */
    void buttonClicked (Button* clicked) override;
    
private:
	PlayButton playButton;
    BackwardsButton backButton;
    ForwardButton forwardButton;
    
    OptionalScopedPointer<GuiControl> parent;
};
#endif /* defined(__MusicPlayer__PlaybackControl__) */
