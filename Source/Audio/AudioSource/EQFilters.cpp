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
}

EQFilters::~EQFilters()
{
}

void EQFilters::setFilter (int frequency, float gain)
{
    switch (frequency) {
        case 32:
            filters[0]->makePeakNotch(sampleRate, frequency, q, gain);
            break;
        case 64:
            filters[1]->makePeakNotch(sampleRate, frequency, q, gain);
            break;
        case 125:
            filters[2]->makePeakNotch(sampleRate, frequency, q, gain);
            break;
        case 250:
            filters[3]->makePeakNotch(sampleRate, frequency, q, gain);
            break;
        case 500:
            filters[4]->makePeakNotch(sampleRate, frequency, q, gain);
            break;
        case 1000:
            filters[5]->makePeakNotch(sampleRate, frequency, q, gain);
            break;
        case 2000:
            filters[6]->makePeakNotch(sampleRate, frequency, q, gain);
            break;
        case 4000:
            filters[7]->makePeakNotch(sampleRate, frequency, q, gain);
            break;
        case 8000:
            filters[8]->makePeakNotch(sampleRate, frequency, q, gain);
            break;
        case 16000:
            filters[9]->makePeakNotch(sampleRate, frequency, q, gain);
            break;
//        case 32:
//            filters[0]->makeBandPass(sampleRate, frequency, q, gain);
//            break;
//        case 64:
//            filters[1]->makeBandPass(sampleRate, frequency, q, gain);
//            break;
//        case 125:
//            filters[2]->makeBandPass(sampleRate, frequency, q, gain);
//            break;
//        case 250:
//            filters[3]->makeBandPass(sampleRate, frequency, q, gain);
//            break;
//        case 500:
//            filters[4]->makeBandPass(sampleRate, frequency, q, gain);
//            break;
//        case 1000:
//            filters[5]->makeBandPass(sampleRate, frequency, q, gain);
//            break;
//        case 2000:
//            filters[6]->makeBandPass(sampleRate, frequency, q, gain);
//            break;
//        case 4000:
//            filters[7]->makeBandPass(sampleRate, frequency, q, gain);
//            break;
//        case 8000:
//            filters[8]->makeBandPass(sampleRate, frequency, q, gain);
//            break;
//        case 16000:
//            filters[9]->makeBandPass(sampleRate, frequency, q, gain);
//            break;
        default:
            break;
    }
}

void EQFilters::setSampleRate(int _sampleRate)
{
    sampleRate = _sampleRate;
    q = 1.8f;
    for (int i = 0; i < numFrequencies; i++)
    {
        filters.add(new BiquadFilter());
        filters[i]->makePeakNotch(sampleRate, 500, q, 1);
    }
}

void EQFilters::applyFilters (float** samples, int numSamples, int numChannels)
{
    for (int channel = 0; channel < numChannels; channel++)
    {
        for (int i = 0; i < filters.size(); i++)
        {
            filters[i]->processSamples(samples[channel], numSamples);
        }
    }
}