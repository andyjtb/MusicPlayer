/*
 *  TransportSlider.cpp
 *  sdaMidiMeter
 *
 *  Created by Andy on 06/01/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */
#include "TransportSlider.h"


TransportSlider::TransportSlider ()
{
    addAndMakeVisible (&transport);
    transport.setRange (0, 10, 0.1);
    transport.setSliderStyle (Slider::LinearHorizontal);
    transport.setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    transport.addListener (this);

	addAndMakeVisible(&timeLabel);
	timeLabel.setText("0:00 ",dontSendNotification);
    timeLabel.setJustificationType(Justification::centredRight);
    timeLabel.setColour (Label::textColourId, Colours::black);
	timeLabel.setFont (Font (27.0f, Font::plain));
    
    addAndMakeVisible(&lengthLabel);
    lengthLabel.setFont (Font ("Helvetica Neue", 27.0f, Font::plain));
    lengthLabel.setJustificationType (Justification::centredLeft);
    lengthLabel.setText("/ 0:00",dontSendNotification);
    lengthLabel.setColour (Label::textColourId, Colour (0xff6e7075));

    
    transport.setEnabled(false);
    
    setSize (300, 60);
}

TransportSlider::~TransportSlider()
{
}

//==============================================================================
void TransportSlider::paint (Graphics& g)
{

}

void TransportSlider::resized()
{
    transport.setBounds (0, 10, 300, 50);
    timeLabel.setBounds (getWidth()/2-80, 0, 80, 25);
    lengthLabel.setBounds(getWidth()/2, 0, 80, 25);	
}

void TransportSlider::setTransportRange (double minimum, double maximum, double interval)
{
    if (!transport.isEnabled())
        transport.setEnabled(true);
    
	transport.setRange(minimum, maximum, interval);
    
    String length = "/ ";
    length << secondsFormatted(maximum);
    lengthLabel.setText(length, dontSendNotification);
}

void TransportSlider::setTransportPosition (double position)
{
	transport.setValue(position, dontSendNotification);
	updateTime(position);
}

void TransportSlider::updateTime(double time)
{	
	timeLabel.setText(secondsFormatted(time),dontSendNotification);	
}

void TransportSlider::sliderValueChanged (Slider* sliderThatWasMoved)
{
	String transportLocation;
	transportLocation << "transportPosition:" << transport.getValue();
	updateTime(transport.getValue());
	sendActionMessage(transportLocation);
	
}

double TransportSlider::getMaximum()
{
    return transport.getMaximum();
}

String TransportSlider::secondsFormatted (double time)
{
    int minutes = time/60;
	int seconds = time - (minutes * 60);
	int tenths = (time-(floor(time)))*10;
	
    if (tenths > 0.5)
        seconds++;
    
	String stringTime;
	if (seconds<10) {
		stringTime <<minutes<<":"<<"0"<<seconds;//<<":"<<tenths<<"\n";
	}
	else {
		stringTime <<minutes<<":"<<seconds;//<<":"<<tenths<<"\n";
	}
    
    return stringTime;
}

