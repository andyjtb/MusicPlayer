/*
 *  InfoBar.cpp
 *  MusicPlayer
 *
 *  Created by Andy on 26/10/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "InfoBar.h"

InfoBar::InfoBar ()
{
    filteredDataList.addListener(this);
    
    addAndMakeVisible(&indicator);
    
    notFound = notRead = false;
}

InfoBar::~InfoBar()
{
}

//==============================================================================
void InfoBar::paint (Graphics& g)
{
    g.setGradientFill (ColourGradient (Colour (0xffe7e9f9),
                                       0.0f, 0.0f,
                                       Colour (0xffeaeaea),
                                       0.0f, 24.0f,
                                       false));
    g.fillRect (0, 0, getWidth(), getHeight());

    g.setColour (Colour (0xff686868));
    g.drawRect (0, 0, getWidth(), getHeight(), 1);

    g.setColour (Colours::black);
    g.drawText (numTracks,
                (getWidth()/2)-250, 0, 150, getHeight(),
                Justification::centredRight, true);
    g.drawText (time,
                (getWidth()/2)-50, 0, 100, getHeight(),
                Justification::centred, true);
    g.drawText (size,
                (getWidth()/2)+100, 0, 100, getHeight(),
                Justification::centredLeft, true);
    
    if (notFound) {
        String notFoundString = currentFile.getFileName();
        notFoundString << " Could not be found";
        g.drawText(notFoundString, getWidth()-400, 0, 400, getHeight(), Justification::centredRight, true);
    }
    
    if (notRead) {
        String notReadString = currentFile.getFileName();
        notReadString << " Could not be read";
        g.drawText(notReadString, getWidth()-400, 0, 400, getHeight(), Justification::centredRight, true);
    }
    
}

void InfoBar::resized()
{
    indicator.setBounds(10, getHeight()/4, 10, getHeight()-5);
}

void InfoBar::updateBar()
{
    int64 sizeInt = 0;
    int64 timeInt = 0;
    
    if (tableSelectedTracks.size() > 1)
    {
        for (int i = 0 ; i < tableSelectedTracks.size(); i++) {
            ValueTree currentTrack = filteredDataList.getChildWithProperty(MusicColumns::columnNames[MusicColumns::ID], tableSelectedTracks[i]);
            
            sizeInt += int(currentTrack.getProperty(MusicColumns::columnNames[MusicColumns::Size]));
            timeInt += int(currentTrack.getProperty(MusicColumns::columnNames[MusicColumns::Length]));
        }   
        
        numTracks = String(tableSelectedTracks.size())+ " of " + String(filteredDataList.getNumChildren())+ " Songs";
    }
    else
    {
        for (int i = 0 ; i < filteredDataList.getNumChildren(); i++) {
            ValueTree currentTrack = filteredDataList.getChild(i);
        
            sizeInt += int(currentTrack.getProperty(MusicColumns::columnNames[MusicColumns::Size]));
            timeInt += int(currentTrack.getProperty(MusicColumns::columnNames[MusicColumns::Length]));
        }
        
        numTracks = String(filteredDataList.getNumChildren())+ " Songs";
    }
    
    size = File::descriptionOfSizeInBytes(sizeInt);
    
    time = RelativeTime::milliseconds(timeInt).getDescription();
    
    repaint();
}

void InfoBar::valueTreeChildOrderChanged (ValueTree &parentTreeWhoseChildrenHaveMoved)
{
    //Everytime the table changes it re-orders/re-creates the filtered data list, including sorting it so this is the most called function
    
    updateBar();
}

void InfoBar::displayFileStatus (File& file, int result)
{
    currentFile = file;
    
    if (result == 1)
    {
        notFound = true;
        notRead = false;
    }
    else if (result == 2)
    {
        notFound = false;
        notRead = true;
    }
    else
        notFound = notRead = false;
    
    updateBar();
}
