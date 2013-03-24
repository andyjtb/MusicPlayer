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
    ValueTree staticValues ("StaticValues");
    
    audioControl = incomingAudioControl;
    
    
    Identifier frequencies [10] = {"32", "64", "125", "250", "500", "1K", "2K", "4K", "8K", "16K"};
    
    //NOT HAPPY, work out how to add this all in 1 line
    frequencyLabels.add("32");
    frequencyLabels.add("64");
    frequencyLabels.add("125");
    frequencyLabels.add("250");
    frequencyLabels.add("500");
    frequencyLabels.add("1K");
    frequencyLabels.add("2K");
    frequencyLabels.add("4K");
    frequencyLabels.add("8K");
    frequencyLabels.add("16K");
    //
    
    addAndMakeVisible (&toggleButton);
    toggleButton.setButtonText ("On");
    
    addAndMakeVisible (&presetCombo);
    presetCombo.addItem("Flat", 1);
    presetCombo.setSelectedId(1);
    presetCombo.addSeparator();
    presetCombo.addItem("Custom", presetCombo.getNumItems()+1);
    presetCombo.addListener (this);

    for (int i = 0; i < frequencyLabels.size(); i++) {
        frequencySliders[i].setRange (0, 2, 0.1);
        frequencySliders[i].setValue(1);
        frequencySliders[i].setSliderStyle (Slider::LinearVertical);
        frequencySliders[i].setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
        frequencySliders[i].addListener (this);
        addAndMakeVisible (&frequencySliders[i]);
    }

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

    g.setColour (Colours::black);
    g.setFont (Font (13.0000f, Font::plain));
    int labelX = 105;
    for (int i = 0; i < frequencyLabels.size(); i++) {
        g.drawText (frequencyLabels[i],
                    labelX, 235, 32, 30,
                    Justification::centred, true);
        labelX += 40;
    }
}

void Equaliser::resized()
{
    toggleButton.setBounds (8, 8, 48, 24);
    presetCombo.setBounds (224, 8, 150, 24);
    
    int sliderX = 104;
    for (int i = 0; i < frequencyLabels.size(); i++) {
        frequencySliders[i].setBounds (sliderX, 42, 31, 200);
        sliderX += 40;
    }
}

void Equaliser::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == &presetCombo)
    {
        if (presetCombo.getSelectedIdAsValue().toString() == "Flat")
        {
            for (int i = 0; i < frequencyLabels.size(); i++) {
                frequencySliders[i].setValue(0);
            }
        }
    }
}

void Equaliser::sliderValueChanged (Slider* sliderThatWasMoved)
{


}

