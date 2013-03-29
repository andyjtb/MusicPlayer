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



class Equaliser  : public Component,
                   public ComboBoxListener,
                   public SliderListener
{
public:
    Equaliser (AudioControl* incomingAudioControl);
    ~Equaliser();
    
    void paint (Graphics& g);
    void resized();

    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
    void sliderValueChanged (Slider* sliderThatWasMoved);
    
private:
    ToggleButton toggleButton;
    ComboBox presetCombo;
    
    String frequencyStrings[numFrequencies];
    int frequencies[numFrequencies];
    Slider frequencySliders[numFrequencies];
    Label frequencyLabels[numFrequencies];
    ValueTree eqSettings;

    OptionalScopedPointer<AudioControl> audioControl;
    OptionalScopedPointer<EQFilters> eqFilters;
};
#endif
