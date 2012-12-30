//
//  TrackArt.cpp
//  MusicPlayer
//
//  Created by Andy on 09/12/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "TrackArt.h"

TrackArt::TrackArt ()
{	   
//    addAndMakeVisible (&albumArt);
//    albumArt.setSize(350, 350);
    addAndMakeVisible(&view);
    view.setViewedComponent(&albumArt, false);
    view.setScrollBarsShown (true, true);
    view.setScrollBarThickness(10);
    
    addAndMakeVisible (&sizeLabel);
    sizeLabel.setFont (Font (20.5000f, Font::plain));
    sizeLabel.setText ("Size:", false);
    
    addAndMakeVisible (&size);
    size.setRange (150, 400, 10);
    size.setSliderStyle (Slider::LinearHorizontal);
    size.setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    size.addListener (this);
    
    addAndMakeVisible (&file);
    file.setButtonText ("Add from File");
    file.setConnectedEdges (Button::ConnectedOnRight);
    file.addListener (this);
    
    addAndMakeVisible (&url);
    url.setButtonText ("Add from URL");
    url.setConnectedEdges (Button::ConnectedOnLeft);
    url.addListener (this);
    
    setSize (550, 440);
}

TrackArt::~TrackArt()
{
	
}

//==============================================================================
void TrackArt::paint (Graphics& g)
{
}

void TrackArt::resized()
{
    view.setBounds (96, 0, 350, 350);
    sizeLabel.setBounds (0, 360, 56, 24);
    size.setBounds (32, 392, 150, 24);
    file.setBounds (216, 392, 150, 24);
    url.setBounds (366, 392, 150, 24);
}

void TrackArt::buttonClicked (Button* buttonThatWasClicked)
{

    if (buttonThatWasClicked == &file)
    {
        albumArt.fromFile();
    }
    else if (buttonThatWasClicked == &url)
    {
        albumArt.fromUrl();
    }

}

void TrackArt::sliderValueChanged (Slider* sliderThatWasMoved)
{
    if (sliderThatWasMoved == &size)
    {
        albumArt.changeSize(size.getValue());
    }
}


void TrackArt::setTrack (ValueTree incomingTrack)
{
    selectedTrack = incomingTrack;
    
    File selectedFile(selectedTrack.getProperty(MusicColumns::columnNames[MusicColumns::Location]));
    
    cover = TagReader::getAlbumArt(selectedFile);
    //albumArt.setSize(view.getWidth(), view.getHeight());
    albumArt.setCover(cover);
    //albumArt.setSize(view.getWidth(), view.getHeight());
    size.setRange (view.getHeight(), cover.getHeight()+500, 10);
    size.setValue(size.getMinimum(), sendNotification);
    
}