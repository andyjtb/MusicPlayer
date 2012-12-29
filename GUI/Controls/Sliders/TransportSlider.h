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

class TransportSlider  : public Component,
                         public ActionBroadcaster,
                         public SliderListener
{
public:
    TransportSlider ();
    ~TransportSlider();

    void paint (Graphics& g);
    void resized();
    
	void setTransportRange (double minimum, double maximum, double interval);
	void setTransportPosition (double position);
	void updateTime(double time);
    
    double getMaximum();

private:
	void sliderValueChanged (Slider* sliderThatWasMoved);
    Slider* transport;
	Label* timeLabel;

	

};


#endif   //H_TRANSPORTSLIDER
