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

#include "JuceHeader.h"

/**
 An audio meter, made using sliders.
 Three sliders are used to cover the audio range, green (0-0.8), yellow (0.8-0.99) and red (0.99-1)
 */
class Meter  : public Component
{
public:
	/**
     Constructor
     */
    Meter ();
	/**
     Destructor
     */
    ~Meter();
	/**
	 Used to control the value displayed on the Meter
	 @param meterValue The value to be displayed by the Meter
	 */
	void externalMeterValue(float meterValue);
	
private:
    /** @internal*/
	void paint (Graphics& g) override;
    /** @internal*/
    void resized() override;
    
    Slider meterLow;
    Slider meterMid;
    Slider meterHigh;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Meter);
	
};


#endif  //H_METER
