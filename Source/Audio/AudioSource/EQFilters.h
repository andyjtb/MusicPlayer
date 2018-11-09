//
//  EQFilters.h
//  MusicPlayer
//
//  Created by Andy on 23/03/2013.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef EQFILTERS 
#define EQFILTERS

#include "JuceHeader.h"
#include "Settings/Settings.h"

#define numFrequencies 10
/** A bank of fixed frequency graphic EQ style filters 
 */
class EQFilters
{
public:
    /** Constructor
     */
    EQFilters ();
	/** Destructor 
     */
    ~EQFilters();
    
    /** Sets the gain of one filter
     @param frequency The frequency of the filter to be altered
     @param gain The gain level (0 - 2)
     */
    void setFilter (int frequency, float gain);
    /** Sets sample rate of all filters (necessary for correct operation)
     @param _sampleRate The sample rate of the audio to be filtered
     */
    void setSampleRate (int _sampleRate);
    
    /** Takes the audio passed to it and applies all of the required equaliser filtering
     @param samples Pointer to the samples to be processed
     @param numSamples The number of samples to be processed
     @param numChannels The number of channels
     */
    void applyFilters(float** samples, int numSamples, int numChannels);
    
private:
    IIRFilter filters[numFrequencies][2];
    float gains[numFrequencies];
    int sampleRate;
    float q;
};
#endif
