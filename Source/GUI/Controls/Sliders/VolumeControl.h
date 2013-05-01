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
/**
 A slider with a volume icon that updates based on the volume being displayed by the slider
 */
class VolumeControl  :  public Component,
                        public SliderListener,
                        public ButtonListener,
						public ActionBroadcaster
{
public:
    /** 
     Constructor
     */
    VolumeControl ();
    /** 
     Destructor
     */
    ~VolumeControl();

    /** 
     @internal
     */
    void paint (Graphics& g);
    /** 
     @internal
     */
    void resized();
    /** 
     Adds the incoming class as a listener to the volume slider's Value
     @param incomingListener The listener to be added to the volume slider listener list
     */
	void addValueListener (Value::Listener* incomingListener);
    /** 
     @return The Value object for the volume slider
     */
	Value& getSliderValue();
	
    /**
     @return The current volume between 0 - 1
     */
    double getVolume();
    /**
     Sets the volume between 0 - 1, triggers listeners who are listening to the slider
     @param incomingVolume The new volume
     */
    void setVolume(double incomingVolume);
    
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
    /**
     Adjusts the volume based on the slider value
     @param sliderThatWasMoved A pointer to the slider that moved, only ever volumeSlider
     */
    void sliderValueChanged (Slider* sliderThatWasMoved);
    /**
     Mutes the volume if the volume icon is clicked
     */
    void buttonClicked (Button* buttonThatWasClicked);
	
	
    Slider volumeSlider;
    ImageButton volumeButton;
	
	Image cachedImage_vol0_png;
    Image cachedImage_vol25_png;
    Image cachedImage_vol50_png;
    Image cachedImage_vol75_png;
    Image cachedImage_volFull_png;

	float volume;	
	float muteVolume;

};


#endif   //H_VOLUMECONTROL
