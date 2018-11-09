/*
 *  TransportSlider.h
 *  sdaMidiMeter
 *
 *  Created by Andy on 06/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef H_TRANSPORTSLIDER
#define H_TRANSPORTSLIDER

#include "JuceHeader.h"
#include <math.h>
#include "iTunesLibrary/MusicLibraryHelpers.h"

/** 
 A slider used as a transport control, also contains two labels, one which displays the current position in the audio file and one which shows the full length
 */
class TransportSlider  : public Component,
                         public ActionBroadcaster,
                         public Slider::Listener
{
public:
    /** 
     Constructor
     */
    TransportSlider ();
    /** 
     Destructor
     */
    ~TransportSlider();
    
    /** 
     @internal
     */
    void paint (Graphics& g) override;
    /** 
     @internal
     */
    void resized() override;

    /** 
     Sets the sliders position and updates the time display labels
     @param position Current playback position
     */
	void setTransportPosition (double position);
    /** 
     */
	//void updateTime(double time);
   
    /** 
     Sets the maximum slider range and right hand time value
     @param _maximum Length of the currently loaded song
     */
    void setMaximum(double _maximum);
    /** 
     @return The maximum length of the currently playing song
     */
    double getMaximum();
    
private:
    /** 
     Formats a time in seconds as a string in the format Minutes:seconds
     @param time A time in seconds
     @return The time formatted as a string minutes:seconds
     */
    String secondsFormatted(double time);
    /** 
     @internal
     */
	void sliderValueChanged (Slider* sliderThatWasMoved) override;
    Slider transport;
	Label timeLabel;
    Label lengthLabel;

	

};


#endif   //H_TRANSPORTSLIDER
