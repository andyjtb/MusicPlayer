/*
 *  AudioControl.h
 *  sdaMidiMeter
 *
 *  Created by Andy Brown on 12/8/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef H_AUDIOCONTROL
#define H_AUDIOCONTROL

#include "../JuceLibraryCode/JuceHeader.h"

#include "TagReader.h"
#include "Settings.h"
#include "EQFilters.h"

#include "dRowAudio_SoundTouchAudioSource.h"
#include "dRowAudio_SoundTouchProcessor.h"

/**
 Top Level Audio Class
 */
class AudioControl  :   public AudioIODeviceCallback,
						public ChangeBroadcaster,
						public ValueListener
{
public:
	//==============================================================================
    /**
     Constructor
     */
	AudioControl();
    
    /**
     Destructor
     */
	~AudioControl();
	
	/**
     Loads the file at the specified location
     */
    int loadFile (const File& audioFile);
    
    
//    bool isPlaying();
    
    /** Sets the pitch 
     @param incomingPitch The value to change the pitch by (0.5 - 1.5, 1 = original)
     */
	void setPitch (const float incomingPitch);
	/** Sets the speed
     @param incomingSpeed The value to change the speed by (0.5 - 1.5, 1 = original)
     */
	void setPlaybackSpeed(const float incomingSpeed);
	
    //AudioCallbacks================================================================
	/** Processes a block of incoming 
     @param inputChannelData A set of arrays containing the audio data for each
     incoming channel
     
     @param numInputChannels The number of pointers to channel data in the
     inputChannelData array.
     
     @param outputChannelData A set of arrays which need to be filled with the data
     that should be sent to each outgoing channel of the device.

     @param numOutputChannels The number of pointers to channel data in the
     outputChannelData array.
     
     @param numSamples The number of samples in each channel of the input and
     output arrays.
     */
	void audioDeviceIOCallback (const float** inputChannelData,
                                int numInputChannels,
                                float** outputChannelData,
                                int numOutputChannels,
                                int numSamples);
    /** Implementation of the AudioSource method.
     */
    void audioDeviceAboutToStart (AudioIODevice* device);
    /** Implementation of the AudioSource method.
     */
    void audioDeviceStopped();
    
    /** Progressively lessens the output meter level so it fades to zero instead of abruptly stopping
     */
    void outputMeterAnimatedToZero();
    /** Provides a means of getting the meter values
     @param channel The channel which you want to get the meter value for "L" or "R"
     @return The meter value
     */
	float getOutputMeterValue(const char* channel);	
	
    /** Displays the audio preferences settings page
     @param centerComponent The component to display the page in the middle of
     */
	void showAudioPreferences(Component* centerComponent);
    
    /** Sets the playback volume
    @param incomingVolume The volume between 0-1
     */
	void setVolume(double incomingVolume);
    /** Returns the current playback volume
     @return The volume
     */
    double getVolume();
	
    /** Returns the position into the current song that is playing
     @return The position of the song in seconds
     */
	double getTransportPosition();
    /** Sets the current playback position for the song currently playing
     @param position The position to starting playing from in seconds
     */
	void setTransportPosition(double position);
    /** Length of the song playing in seconds
     @return Length of song
     */
	double getTransportLength();
	
    /** Callback listening to the singletonPlayState variable, stops or starts playing back audio based on the value of that variable
     @param valueChanged The Value which changed to trigger this callback, in this case always singletonPlayState
     */
	void valueChanged (Value& valueChanged);
    
    /** The value which represents whether the audio is to have the eq filters applied or not
     @return Whether to apply eq
     */
    Value& getApplyEQ() {return applyEQ;}
    /** A pointer to the bank of EQ filters
     */
    EQFilters* getEqFilters() {return &eqFilters;}
    
    /** Returns the current settings being used by the SoundTouch Processor, used for speed and pitch
     */
    SoundTouchProcessor::PlaybackSettings getSoundTouchSettings (){return soundTouchSettings;}
    
    /** Returns a string containing all of the file extensions that the audio format manager is able to create readers for, i.e what audio file the application can play
     */
    String getRegisteredFormatWildcard() {return formatManager.getWildcardForAllFormats();}
    
private:
    
    AudioDeviceManager audioDeviceManager;	// this wraps the actual audio device
	AudioSourcePlayer audioSourcePlayer;
	AudioFormatManager formatManager;
    
    TimeSliceThread bufferingThread;
	AudioTransportSource transport;
	
    EQFilters eqFilters;
	ScopedPointer<AudioFormatReaderSource> currentAudioFileSource;
	ScopedPointer<SoundTouchAudioSource> soundTouch;
    ScopedPointer<BufferingAudioSource> bufferingAudioSource;
    //SoundTouchAudioSource soundTouch;
	SoundTouchProcessor::PlaybackSettings soundTouchSettings;
	
	//audio variables
    float audioMeterOutputL;
	float audioMeterOutputR;
    
    //Shared Memory Lock
    CriticalSection sharedMemory;
    float sharedMeterOutputL;
	float sharedMeterOutputR;
    
    Value applyEQ;
};



#endif //H_AUDIO