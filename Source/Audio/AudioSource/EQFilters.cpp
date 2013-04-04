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
    sampleRate = 44100;
    q = 1.8f;
    
    filterID = 1;
}

EQFilters::~EQFilters()
{
}

void EQFilters::setFilter (int frequency, float gain)
{
    switch (frequency) {
        case 32:
            for (int i = 0; i < 2; i++)
                filters[0][i].makePeakNotch(sampleRate, frequency, q, gain);
            break;
        case 64:
            for (int i = 0; i < 2; i++)
            filters[1][i].makePeakNotch(sampleRate, frequency, q, gain);
            break;
        case 125:
            for (int i = 0; i < 2; i++)
            filters[2][i].makePeakNotch(sampleRate, frequency, q, gain);
            break;
        case 250:
            for (int i = 0; i < 2; i++)
            filters[3][i].makePeakNotch(sampleRate, frequency, q, gain);
            break;
        case 500:
            for (int i = 0; i < 2; i++)
            filters[4][i].makePeakNotch(sampleRate, frequency, q, gain);
            break;
        case 1000:
            for (int i = 0; i < 2; i++)
            filters[5][i].makePeakNotch(sampleRate, frequency, q, gain);
            break;
        case 2000:
            for (int i = 0; i < 2; i++)
            filters[6][i].makePeakNotch(sampleRate, frequency, q, gain);
            break;
        case 4000:
            for (int i = 0; i < 2; i++)
            filters[7][i].makePeakNotch(sampleRate, frequency, q, gain);
            break;
        case 8000:
            for (int i = 0; i < 2; i++)
            filters[8][i].makePeakNotch(sampleRate, frequency, q, gain);
            break;
        case 16000:
            for (int i = 0; i < 2; i++)
            filters[9][i].makePeakNotch(sampleRate, frequency, q, gain);
            break;
        default:
            break;
    }
}

void EQFilters::setSampleRate(int _sampleRate)
{
    sampleRate = _sampleRate;
    for (int i = 0; i < numFrequencies; i++)
    {
        for (int j = 0; j < 2; j++)
            filters[i][j].makePeakNotch(sampleRate, 500, q, 1);
    }
}

void EQFilters::setFilterID (int currentFilter)
{
    filterID = currentFilter;
}

int EQFilters::getFilterID ()
{
    return filterID;
}

void EQFilters::applyFilters (float** samples, int numSamples, int numChannels)
{
    //for (int channel = 0; channel < numChannels; channel++)
    //{
        for (int i = 0; i < numFrequencies; i++)
        {
            filters[i][0].processSamples(samples[0], numSamples);
            filters[i][1].processSamples(samples[1], numSamples);
        }
    //}
}