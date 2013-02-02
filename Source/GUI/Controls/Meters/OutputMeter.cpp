/*
 *  OutputMeter.cpp
 *  sdaMidiMeter
 *
 *  Created by Andy on 03/02/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "OutputMeter.h"

OutputMeter::OutputMeter()
{
    addAndMakeVisible (&outputLabel);
	outputLabel.setText("Output", false);
    outputLabel.setFont (Font (26.2000f, Font::plain));
    outputLabel.setJustificationType (Justification::centredLeft);
    outputLabel.setEditable (false, false, false);
    outputLabel.setColour (TextEditor::textColourId, Colours::black);
    outputLabel.setColour (TextEditor::backgroundColourId, Colour (0x0));
	
	addAndMakeVisible(&meterL);	
	addAndMakeVisible(&meterR);
	
    setSize (250, 80);
}

OutputMeter::~OutputMeter()
{
	
}

//==============================================================================
void OutputMeter::paint (Graphics& g)
{
    //g.fillAll (Colours::white.withAlpha (0.0f));
	
}

void OutputMeter::resized()
{
    meterL.setBounds (24, 32, 225, 24);
	meterR.setBounds (24, 54, 225, 24);
    outputLabel.setBounds (0, 0, 78, 24);
}

void OutputMeter::setOutputMeterValueL(float incomingMeterValue)
{
	meterL.externalMeterValue(incomingMeterValue);
}
void OutputMeter::setOutputMeterValueR(float incomingMeterValue)
{
	meterR.externalMeterValue(incomingMeterValue);
}