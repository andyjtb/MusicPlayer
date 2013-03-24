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

class EQFilters
{
public:
    EQFilters ();
	
    ~EQFilters();
    
    void applyFilters(float* samples, int numSamples);
    
private:
    OwnedArray<IIRFilter> filters;
};
#endif
