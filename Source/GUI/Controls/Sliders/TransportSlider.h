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

#include "../JuceLibraryCode/JuceHeader.h"
#include <math.h>
#include "MusicLibraryHelpers.h"

/** A slider used as a transport control, also contains two labels, one which displays the current position in the audio file and one which shows the full length
 */
class TransportSlider  : public Component,
                         public ActionBroadcaster,
                         public SliderListener
{
public:
    /** Constructor
     */
    TransportSlider ();
    /** Destructor
     */
    ~TransportSlider();
    
    /** @internal
     */
    void paint (Graphics& g);
    /** @internal
     */
    void resized();

    /** Sets the sliders position and updates the time display labels
     */
	void setTransportPosition (double position);
    /** 
     */
	//void updateTime(double time);
   
    /** 
     */
    void setMaximum(double _maximum);
    /**
     */
    double getMaximum();
    
private:
    /** 
     */
    String secondsFormatted(double time);
    /** @internal
     */
	void sliderValueChanged (Slider* sliderThatWasMoved);
    Slider transport;
	Label timeLabel;
    Label lengthLabel;

	

};


#endif   //H_TRANSPORTSLIDER
