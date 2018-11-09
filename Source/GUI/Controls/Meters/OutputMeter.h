/*
 *  OutputMeter.h
 *  sdaMidiMeter
 *
 *  Created by Andy on 03/02/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef H_OUTPUTMETER
#define H_OUTPUTMETER

#include "JuceHeader.h"
#include "Meter.h"

/**
 A class containg two Meter s.
 Used for displaying the output level to the chosen ooutput device.
 @see Meter
 */
class OutputMeter  : public Component
{
public:
	/**
     Constructor
     */
    OutputMeter();
	/**
     Destructor
     */
    ~OutputMeter();
	/**
	 Used to control the value displayed on the left Meter
	 @param incomingMeterValue The value to be displayed by the left output Meter
	 */
	void setOutputMeterValueL (float incomingMeterValue);
	/**
	 Used to control the value displayed on the right Meter
	 @param incomingMeterValue The value to be displayed by the right output Meter
	 */
	void setOutputMeterValueR (float incomingMeterValue);
	
private:
    /** @internal */
    void paint (Graphics& g) override;
    /** @internal */
    void resized() override;

    Label outputLabel;
	Meter meterL;
	Meter meterR;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OutputMeter);
};


#endif  //H_OUTPUTMETER
