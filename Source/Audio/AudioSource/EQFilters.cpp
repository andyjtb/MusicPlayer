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
    sampleRate = 1;
    q = 1.8f;
    
    //Fills gains array with 1
    std::fill_n(gains, numFrequencies, 1);
}

EQFilters::~EQFilters()
{
}

void EQFilters::setFilter (int frequency, float gain)
{
    for (int i = 0; i < 2; i++)
    {
        IIRCoefficients co = IIRCoefficients::makePeakFilter(sampleRate, frequency, q, gain);
        
        switch (frequency) {
            case 32:
                currentEqDetails.gains.set(0, gain);
                filters[0][i].setCoefficients(co);
                break;
            case 64:
                currentEqDetails.gains.set(1, gain);
                filters[1][i].setCoefficients(co);
                break;
            case 125:
                currentEqDetails.gains.set(2, gain);
                filters[2][i].setCoefficients(co);
                break;
            case 250:
                currentEqDetails.gains.set(3, gain);
                filters[3][i].setCoefficients(co);
                break;
            case 500:
                currentEqDetails.gains.set(4, gain);
                filters[4][i].setCoefficients(co);
                break;
            case 1000:
               currentEqDetails.gains.set(5, gain);
                filters[5][i].setCoefficients(co);
                break;
            case 2000:
                currentEqDetails.gains.set(6, gain);
                filters[6][i].setCoefficients(co);
                break;
            case 4000:
                currentEqDetails.gains.set(7, gain);
                filters[7][i].setCoefficients(co);
                break;
            case 8000:
                currentEqDetails.gains.set(8, gain);
                filters[8][i].setCoefficients(co);
                break;
            case 16000:
                currentEqDetails.gains.set(9, gain);
                filters[9][i].setCoefficients(co);
                break;
            default:
                break;
        }
    }
}

void EQFilters::setSampleRate(int _sampleRate)
{
    if (_sampleRate != sampleRate)
    {
        sampleRate = _sampleRate;
        for (int i = 0; i < numFrequencies; i++)
        {
            IIRCoefficients co = IIRCoefficients::makePeakFilter(sampleRate, 500, q, currentEqDetails.gains[i]);
            for (int j = 0; j < 2; j++)
                filters[i][j].setCoefficients(co);
        }
    }
}

void EQFilters::applyFilters (float** samples, int numSamples, int numChannels)
{
    for (int i = 0; i < numFrequencies; i++)
    {
        //Can't use the same filter for both channels
        filters[i][0].processSamples(samples[0], numSamples);
        filters[i][1].processSamples(samples[1], numSamples);
    }
}
