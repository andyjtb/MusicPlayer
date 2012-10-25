/*
 *  Meter.h
 *  sdaMidiMeter
 *
 *  Created by Andy on 06/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef H_METER
#define H_METER

#include "../JuceLibraryCode/JuceHeader.h"

class Meter  : public Component
               
{
public:
   
    Meter ();
    ~Meter();


    void paint (Graphics& g);
    void resized();
	void externalMeterValue(float meterValue);
	


private:
	

    Slider meterLow;
    Slider meterMid;
    Slider meterHigh;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Meter);
	
};


#endif  //H_METER
