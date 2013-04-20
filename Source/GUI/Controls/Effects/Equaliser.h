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



class Equaliser  : public Component,
                   public ComboBoxListener,
                   public SliderListener,
                   public ButtonListener
{
public:
    Equaliser (AudioControl* incomingAudioControl);
    ~Equaliser();
    
    void paint (Graphics& g);
    void resized();

    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
    void sliderValueChanged (Slider* sliderThatWasMoved);
    
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
