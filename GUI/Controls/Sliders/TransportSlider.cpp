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
    addAndMakeVisible (transport = new Slider);
    transport->setRange (0, 10, 0.1);
    transport->setSliderStyle (Slider::LinearHorizontal);
    transport->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    transport->addListener (this);

	addAndMakeVisible(timeLabel = new Label);
	timeLabel->setText("0:00:0",false);
	timeLabel->setFont (Font (27.4000f, Font::plain));
	
    setSize (300, 60);
}

TransportSlider::~TransportSlider()
{
	deleteAndZero(transport);
	deleteAndZero(timeLabel);
}

//==============================================================================
void TransportSlider::paint (Graphics& g)
{

}

void TransportSlider::resized()
{
    transport->setBounds (0, 10, 300, 50);
	timeLabel->setBounds (104, 0, 300, 24);
	
}

void TransportSlider::setTransportRange (double minimum, double maximum, double interval)
{
	transport->setRange(minimum, maximum, interval);
}

void TransportSlider::setTransportPosition (double position)
{
	transport->setValue(position, sendNotificationAsync);
	updateTime(position);
}

void TransportSlider::updateTime(double time)
{
	//float time = transport->getValue();
	
	int minutes = time/60;
	int seconds = time - (minutes * 60);
	//seconds = time%60;
	int tenths = (time-(floor(time)))*10;
	
	String stringTime;
	if (seconds<10) {
		stringTime <<minutes<<":"<<"0"<<seconds<<":"<<tenths<<"\n";
	}
	else {
		stringTime <<minutes<<":"<<seconds<<":"<<tenths<<"\n";
	}
	
	timeLabel->setText(stringTime,false);	
}

void TransportSlider::sliderValueChanged (Slider* sliderThatWasMoved)
{
	String transportLocation;
	transportLocation << "transportPosition:" << transport->getValue();
	setTransportPosition(transport->getValue());
	sendActionMessage(transportLocation);
}



