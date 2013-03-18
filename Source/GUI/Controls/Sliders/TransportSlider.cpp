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
	timeLabel.setText("0:00:0",false);
	timeLabel.setFont (Font (27.4000f, Font::plain));
    
    //addAndMakeVisible(&lengthLabel);
    lengthLabel.setText("/ 0:00", false);
	lengthLabel.setFont (Font (27.4000f, Font::plain));
    
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
    timeLabel.setBounds (104, 0, 85, 24);
	//timeLabel.setBounds (50, 0, 85, 24);
    //lengthLabel.setBounds(130, 0, 110, 24);	
}

void TransportSlider::setTransportRange (double minimum, double maximum, double interval)
{
	transport.setRange(minimum, maximum, interval);
    
    String length = "/ ";
    length << TimeHelpers::secondsToTimeLength(maximum);
    //DBG("Maximum = " << maximum);
    lengthLabel.setText("/ "+TimeHelpers::secondsToTimeLength(maximum), false);
}

void TransportSlider::setTransportPosition (double position)
{
	transport.setValue(position, dontSendNotification);
	updateTime(position);
}

void TransportSlider::updateTime(double time)
{
	int minutes = time/60;
	int seconds = time - (minutes * 60);
	int tenths = (time-(floor(time)))*10;
	
	String stringTime;
	if (seconds<10) {
		stringTime <<minutes<<":"<<"0"<<seconds<<":"<<tenths<<"\n";
	}
	else {
		stringTime <<minutes<<":"<<seconds<<":"<<tenths<<"\n";
	}
	
	timeLabel.setText(stringTime,false);	
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


