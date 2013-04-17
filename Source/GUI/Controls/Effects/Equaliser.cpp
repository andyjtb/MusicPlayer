/*
 *  Equaliser.cpp
 *  MusicPlayer
 *
 *  Created by Andy on 14/03/2013.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */
#include "Equaliser.h"


Equaliser::Equaliser (AudioControl* incomingAudioControl) :
    frequencies ({32,64,125,250,500,1000,2000,4000,8000,16000}),
    frequencyStrings({"32", "64", "125", "250", "500", "1K", "2K", "4K", "8K", "16K"})
{
    
    audioControl.set(incomingAudioControl, false);
    eqFilters.set(audioControl->getEqFilters(), false);
    
    addAndMakeVisible (&toggleButton);
    toggleButton.setButtonText ("On");
    toggleButton.addListener(this);
    
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
    for (int i = 0; i < singletonEqSettings.getNumChildren(); i++)
        presetCombo.addItem(singletonEqSettings.getChild(i).getProperty("Name").toString(), presetCombo.getNumItems()+1);
    presetCombo.addListener (this);

    for (int i = 0; i < numFrequencies; i++) {
        frequencySliders[i].setName(String(frequencies[i]));
        frequencySliders[i].setRange (0.1, 1.9, 0.1);
        frequencySliders[i].setSliderStyle (Slider::LinearVertical);
        frequencySliders[i].setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
        frequencySliders[i].addListener (this);
        addAndMakeVisible (&frequencySliders[i]);
        
        frequencyLabels[i].setText(frequencyStrings[i], dontSendNotification);
        addAndMakeVisible(&frequencyLabels[i]);
    }
    
    //Waits till all sliders have been created then triggers the combobox change callback to set them all to the incoming value
    presetCombo.setSelectedId(currentEqDetails.ID);
    
    toggleButton.setToggleState(currentEqDetails.On.getValue(), false);
    
    File eqTest = File(File::getSpecialLocation(File::userMusicDirectory).getFullPathName() + "/MusicPlayer/EQ.xml");
    writeValueTreeToFile(singletonEqSettings, eqTest);
    
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
        if (presetCombo.getText() == "Flat")
        {
            for (int i = 0; i < numFrequencies; i++) {
                frequencySliders[i].setEnabled(false);
                frequencySliders[i].setValue(1);
            }
        }
        else
        {
            ValueTree currentSettings = singletonEqSettings.getChildWithProperty("Name", presetCombo.getText());
            
            for (int i = 0; i < numFrequencies; i++) {
                frequencySliders[i].setEnabled(true);
                frequencySliders[i].setValue(currentSettings.getProperty(String(frequencies[i])));
            }
        }
        //eqFilters->setFilterID(presetCombo.getSelectedId());
        currentEqDetails.ID = presetCombo.getSelectedId();
    }
}

void Equaliser::sliderValueChanged (Slider* sliderThatWasMoved)
{
    eqFilters->setFilter(sliderThatWasMoved->getName().getIntValue(), sliderThatWasMoved->getValue());
    
    if (presetCombo.getText() != "Flat")
    {
        ValueTree currentlySelected = singletonEqSettings.getChildWithProperty("Name", presetCombo.getText());
        currentlySelected.setProperty(sliderThatWasMoved->getName(), sliderThatWasMoved->getValue(), 0);
    }
}

void Equaliser::buttonClicked (Button* button)
{
    if (button == &add)
    {
        AlertWindow addPopup("Add EQ Setting", "Enter the name for your new EQ Setting", AlertWindow::NoIcon);
        TextEditor addText;
        
        addText.setBounds(getWidth()/2, getHeight()/2, 300, 25);
        addText.setText("New Setting");
        
        addPopup.addCustomComponent(&addText);
        
        addPopup.addButton("Cancel", 0);
        addPopup.addButton("Ok", 1);
        
        if (addPopup.runModalLoop() != 0) {
            String newName = addText.getText();
            //Checks to see if it exists
            if (!singletonEqSettings.getChildWithProperty("Name", newName).isValid())
                {
            ValueTree newSetting ("SETTING");
            newSetting.setProperty("Name", addText.getText(), 0);
            for (int i = 0; i < numFrequencies; i++) {
                newSetting.setProperty(String(frequencies[i]), 1.0, 0);
            }
            singletonEqSettings.addChild(newSetting, -1, 0);
            
            int index = presetCombo.getNumItems()+1;
            presetCombo.addItem(newSetting.getProperty("Name"), index);
            presetCombo.setSelectedId(index);
                }
            else
            {
                //Exists
                AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "EQ Setting Already Exists", "An EQ Setting with that title already exists\n Please choose another name");
            }
        }
    }
    else if (button == &minus)
    {
        singletonEqSettings.removeChild(singletonEqSettings.getChildWithProperty("Name", presetCombo.getText()), 0);
        //Clear and re add everything as Jules has removed the removeItem function
        //presetCombo.removeItem(presetCombo.getSelectedId());
        presetCombo.clear();
        presetCombo.addItem("Flat", 1);
        presetCombo.addSeparator();
        for (int i = 0; i < singletonEqSettings.getNumChildren(); i++)
            presetCombo.addItem(singletonEqSettings.getChild(i).getProperty("Name").toString(), presetCombo.getNumItems()+1);
        presetCombo.setSelectedId(1);
        
    }
    
    else if (button == &toggleButton)
    {
        currentEqDetails.On = !currentEqDetails.On.getValue();
        toggleButton.setToggleState(currentEqDetails.On.getValue(), false);
    }
}
