/*
 *  Meter.cpp
 *  sdaMidiMeter
 *
 *  Created by Andy on 06/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */
#include "Meter.h"

Meter::Meter ()
{
    addAndMakeVisible (&meterLow);
    meterLow.setRange (0, 0.80, 0.0001);
    meterLow.setSliderStyle (Slider::LinearBar);
    meterLow.setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
	meterLow.setColour (Slider::backgroundColourId, Colour (0x70006000));
    meterLow.setColour (Slider::thumbColourId, Colour (0xff39ff34));
    meterLow.setColour (Slider::rotarySliderOutlineColourId, Colour (0x0));
    meterLow.setColour (Slider::textBoxOutlineColourId, Colour (0x808080));
	meterLow.setValue(0, dontSendNotification);
   // meterLow.addListener (this);

    addAndMakeVisible (&meterMid);
	meterMid.setRange (0.8001, 0.99, 0.0001);
    meterMid.setSliderStyle (Slider::LinearBar);
    meterMid.setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
	meterMid.setColour (Slider::backgroundColourId, Colour (0x705d6000));
    meterMid.setColour (Slider::thumbColourId, Colour (0xfff8ff00));
    meterMid.setColour (Slider::rotarySliderOutlineColourId, Colour (0x0));
    meterMid.setColour (Slider::textBoxOutlineColourId, Colour (0x808080));
	meterMid.setValue(0, dontSendNotification);
    //meterMid.addListener (this);

    addAndMakeVisible (&meterHigh);
	meterHigh.setRange (0.991, 1., 0.0001);
    meterHigh.setSliderStyle (Slider::LinearBar);
    meterHigh.setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
	meterHigh.setColour (Slider::backgroundColourId, Colour (0x70600008));
    meterHigh.setColour (Slider::thumbColourId, Colours::red);
    meterHigh.setColour (Slider::rotarySliderOutlineColourId, Colour (0x0));
    meterHigh.setColour (Slider::textBoxOutlineColourId, Colour (0x808080));
	meterHigh.setValue(0, dontSendNotification);
    //meterHigh.addListener (this);

    setSize (225, 25);

}

Meter::~Meter()
{
//    deleteAndZero (meterLow);
//    deleteAndZero (meterMid);
//    deleteAndZero (meterHigh);
}

//==============================================================================
void Meter::paint (Graphics& g)
{

}

void Meter::resized()
{
    meterLow.setBounds (0, 0, 150, 24);
    meterMid.setBounds (150, 0, 70, 24);
    meterHigh.setBounds (220, 0, 5, 24);

}

void Meter::externalMeterValue(float meterValue)
{
	meterLow.setValue(meterValue, dontSendNotification);
	meterMid.setValue(meterValue, dontSendNotification);
	meterHigh.setValue(meterValue, dontSendNotification);
}

