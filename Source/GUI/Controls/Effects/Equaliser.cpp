/*
 *  Equaliser.cpp
 *  MusicPlayer
 *
 *  Created by Andy on 14/03/2013.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */
#include "Equaliser.h"


Equaliser::Equaliser (AudioControl* incomingAudioControl) : eqSettings("EQSETTINGS")
{
    
    audioControl.set(incomingAudioControl, false);
    eqFilters.set(audioControl->getEqFilters(), false);
    
    String frequencyStrings [numFrequencies] = {"32", "64", "125", "250", "500", "1K", "2K", "4K", "8K", "16K"};
    int frequencies[numFrequencies] = {32,64,125,250,500,1000,2000,4000,8000,16000};
    
    addAndMakeVisible (&toggleButton);
    toggleButton.setButtonText ("On");
    
    addAndMakeVisible (&add);
    add.setButtonText ("+");
    add.setConnectedEdges(Button::ConnectedOnLeft);
    add.addListener(this);
    
    addAndMakeVisible (&minus);
    minus.setButtonText ("-");
    minus.setConnectedEdges(Button::ConnectedOnRight);
    minus.addListener(this);
    
    addAndMakeVisible (&presetCombo);
    presetCombo.addItem("Flat", 1);
    presetCombo.addSeparator();
    for (int i = 0; i < eqSettings.getNumChildren(); i++)
        presetCombo.addItem(eqSettings.getChild(i).getProperty("Name"), presetCombo.getNumItems()+1);
    presetCombo.addListener (this);

    for (int i = 0; i < numFrequencies; i++) {
        frequencySliders[i].setName(String(frequencies[i]));
        frequencySliders[i].setRange (0.1, 1.9, 0.1);
        frequencySliders[i].setSliderStyle (Slider::LinearVertical);
        frequencySliders[i].setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
        frequencySliders[i].addListener (this);
        addAndMakeVisible (&frequencySliders[i]);
        
        frequencyLabels[i].setText(frequencyStrings[i], false);
        addAndMakeVisible(&frequencyLabels[i]);
    }
    
    //Waits till all sliders have been created then triggers the combobox change callback to set them all to the incoming value
    presetCombo.setSelectedId(eqFilters->getFilterID());
    
    toggleButton.getToggleStateValue().referTo(audioControl->getApplyEQ());
    
    setSize (550, 275);

}

Equaliser::~Equaliser()
{
}

//==============================================================================
void Equaliser::paint (Graphics& g)
{
    g.fillAll (Colours::white);

    g.setColour (Colours::black);
    g.setFont (Font (15.0000f, Font::plain));
    g.drawText ("+12dB",
                40, 35, 48, 30,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (15.0000f, Font::plain));
    g.drawText ("0dB",
                50, 125, 48, 30,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (15.0000f, Font::plain));
    g.drawText ("-12dB",
                40, 215, 48, 30,
                Justification::centred, true);

    g.setColour (Colour (0x18858585));
    for (int lineCounter = 50; lineCounter < 240; lineCounter += 30)
    {
        g.fillRect (109, lineCounter, 380, 2);
    }

}

void Equaliser::resized()
{
    toggleButton.setBounds (8, 8, 48, 24);
    presetCombo.setBounds (224, 8, 150, 24);
    
    minus.setBounds (440, 8, 35, 25);
    add.setBounds (475, 8, 35,25);
    
    int sliderX = 104;
    int labelX = 105;
    for (int i = 0; i < numFrequencies; i++) {
        frequencySliders[i].setBounds (sliderX, 42, 31, 200);
        sliderX += 40;
        
        frequencyLabels[i].setBounds(labelX, 235, 32, 30);
        labelX += 40;
    }
}

void Equaliser::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == &presetCombo)
    {
        if (presetCombo.getSelectedIdAsValue().toString() == "Flat")
        {
            for (int i = 0; i < numFrequencies; i++) {
                //frequencySliders[i].setEnabled(false);
                frequencySliders[i].setValue(1);
                frequencySliders[i].removeListener(this);
            }
        }
        else
        {
            for (int i = 0; i < numFrequencies; i++) {
                frequencySliders[i].addListener(this);
                //frequencySliders[i].setValue();
            }
        }
        eqFilters->setFilterID(presetCombo.getSelectedId());
    }
}

void Equaliser::sliderValueChanged (Slider* sliderThatWasMoved)
{
    eqFilters->setFilter(sliderThatWasMoved->getName().getIntValue(), sliderThatWasMoved->getValue());
}

void Equaliser::buttonClicked (Button* button)
{
    if (button == &add)
    {
        
    }
    else if (button == &minus)
    {
        
    }
}
