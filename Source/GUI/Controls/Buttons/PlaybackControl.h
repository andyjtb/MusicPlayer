//
//  PlaybackControl.h
//  MusicPlayer
//
//  Created by Andy on 29/04/2013.
//
//

#ifndef __MusicPlayer__PlaybackControl__
#define __MusicPlayer__PlaybackControl__

#include "../JuceLibraryCode/JuceHeader.h"
#include "PlayButton.h"
#include "ForwardButton.h"
#include "BackwardsButton.h"

class GuiControl;

class PlaybackControl : public Component,
                        public Button::Listener
{
public:
	//==============================================================================
    /**
     Constructor
     */
	PlaybackControl(GuiControl* _parent);
    
    /**
     Destructor
     */
	~PlaybackControl();
    
    void paint(Graphics& g);
	void resized();
    
    void buttonClicked (Button* clicked);
    
private:
	PlayButton playButton;
    BackwardsButton backButton;
    ForwardButton forwardButton;
    
    OptionalScopedPointer<GuiControl> parent;
};
#endif /* defined(__MusicPlayer__PlaybackControl__) */
