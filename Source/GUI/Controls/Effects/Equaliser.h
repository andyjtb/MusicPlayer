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

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioControl.h"
#include "EQFilters.h"
#include "Settings.h"

/**
 The equaliser controls
 */

class Equaliser  : public Component,
                   public ComboBoxListener,
                   public SliderListener,
                   public ButtonListener
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
    void paint (Graphics& g);
    /** @internal */
    void resized();

    /** Called when the user changes the eq preset combo box
     */
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
    /** Called when the user moves one of the eq sliders, changes the EQFilter stored in AudioControl
     @see AudioControl, EQFilters
     */
    void sliderValueChanged (Slider* sliderThatWasMoved);
    
    /** 
     Adds or deletes an Eq preset
     */
    void buttonClicked (Button* button);
    
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
