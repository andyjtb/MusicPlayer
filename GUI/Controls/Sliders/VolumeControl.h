/*
 *  VolumeControl.h
 *  sdaMidiMeter
 *
 *  Created by Andy on 06/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef H_VOLUMECONTROL
#define H_VOLUMECONTROL

#include "../JuceLibraryCode/JuceHeader.h"

class VolumeControl  :  public Component,
                        public SliderListener,
                        public ButtonListener,
						public ActionBroadcaster
{
public:

    VolumeControl ();
    ~VolumeControl();


    void paint (Graphics& g);
    void resized();


    // Binary resources:
    static const char* vol0_png;
    static const int vol0_pngSize;
    static const char* vol25_png;
    static const int vol25_pngSize;
    static const char* vol50_png;
    static const int vol50_pngSize;
    static const char* vol75_png;
    static const int vol75_pngSize;
    static const char* volFull_png;
    static const int volFull_pngSize;

	
	
private:
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void buttonClicked (Button* buttonThatWasClicked);
	
	
    Slider* VolumeSlider;
    ImageButton* VolumeButton;
	
	Image cachedImage_vol0_png;
    Image cachedImage_vol25_png;
    Image cachedImage_vol50_png;
    Image cachedImage_vol75_png;
    Image cachedImage_volFull_png;

	float volume;	
	float muteVolume;

};


#endif   //H_VOLUMECONTROL
