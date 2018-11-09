/*
 *  Equaliser.h
 *  MusicPlayer
 *
 *  Created by Andy on 26/10/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef EQUALISER
#define EQUALISER

#include "JuceHeader.h"
#include "AudioControl.h"
#include "AudioSource/EQFilters.h"
#include "Settings/Settings.h"

/**
 The equaliser controls
 */

class Equaliser  : public Component,
                   public ComboBox::Listener,
                   public Slider::Listener,
                   public Button::Listener
{
public:
    /**
     Constructor - Takes a pointer to the AudioControl class, allows the EqFilters to be edited directly by this class
     */
    Equaliser (AudioControl* incomingAudioControl);
    /**
     Destructor
     */
    ~Equaliser();
    
    /** Draws the text labels and measurement lines
     */
    void paint (Graphics& g) override;
    /** @internal */
    void resized() override;

    /** Called when the user changes the eq preset combo box
     */
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    /** Called when the user moves one of the eq sliders, changes the EQFilter stored in AudioControl
     @see AudioControl, EQFilters
     */
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    
    /** 
     Adds or deletes an Eq preset
     */
    void buttonClicked (Button* button) override;
    
private:
    ToggleButton toggleButton;
    ComboBox presetCombo;
    
    static String frequencyStrings[numFrequencies];
    static int frequencies[numFrequencies];
    Slider frequencySliders[numFrequencies];
    Label frequencyLabels[numFrequencies];
    
    TextButton add, minus;

    OptionalScopedPointer<AudioControl> audioControl;
    OptionalScopedPointer<EQFilters> eqFilters;
};
#endif
