//
//  EQFilters.cpp
//  MusicPlayer
//
//  Created by Andy on 23/03/2013.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "EQFilters.h"

//========================================================================
EQFilters::EQFilters ()
{
    for (int i = 0; i < 10; i++)
        filters.add(new IIRFilter());
}

EQFilters::~EQFilters()
{
}

void EQFilters::applyFilters (float* samples, int numSamples)
{
    for (int i = 0; i < filters.size(); i++)
    {
        filters[i]->processSamples(samples, numSamples);
    }
}