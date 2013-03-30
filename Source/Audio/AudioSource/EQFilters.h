//
//  EQFilters.h
//  MusicPlayer
//
//  Created by Andy on 23/03/2013.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef EQFILTERS 
#define EQFILTERS

#include "../JuceLibraryCode/JuceHeader.h"
#include "dRowAudio_BiquadFilter.h"

#define numFrequencies 10

class EQFilters
{
public:
    EQFilters ();
	
    ~EQFilters();
    
    void setFilter (int frequency, float gain);
    void setSampleRate (int _sampleRate);
    void applyFilters(float** samples, int numSamples, int numChannels);
    
private:
    OwnedArray<BiquadFilter> filters;
    
    int sampleRate;
    float q;
};
#endif
