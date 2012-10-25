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

#include "../JuceLibraryCode/JuceHeader.h"
#include "Meter.h"

class OutputMeter : public Component
{
public:
    OutputMeter();
    ~OutputMeter();
	
    void paint (Graphics& g);
    void resized();
	void setOutputMeterValueL (float incomingMeterValue);
	void setOutputMeterValueR (float incomingMeterValue);
	
private:
	
    Label outputLabel;
	Meter meterL;
	Meter meterR;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OutputMeter);
};


#endif  //H_OUTPUTMETER
