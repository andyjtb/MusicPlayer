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
    void loadFile (const File& audioFile);
    
    void reloadFile (const File& audioFile, bool couldBeFound);
    
    /**
     Sets the play state
     */
//    void setPlayState(bool shouldPlay);
    
    /**
     Toggles the current play state
     */
//    void togglePlayState();
    
    bool isPlaying();

	void setPitch (const float incomingPitch);
	
	void setPlaybackSpeed(const float incomingSpeed);
	
    //AudioCallbacks================================================================
	
	void audioDeviceIOCallback (const float** inputChannelData,
                                int numInputChannels,
                                float** outputChannelData,
                                int numOutputChannels,
                                int numSamples);
    
    void audioDeviceAboutToStart (AudioIODevice* device);
    void audioDeviceStopped();
	float getOutputMeterValue(const char* channel);	
	
	void showAudioPreferences(Component* centerComponent);
    
	void setVolume(double incomingVolume);
    double getVolume();
	
	double getTransportPosition();
	void setTransportPosition(double position);
	double getTransportLength();
	
	void valueChanged (Value& valueChanged);
    
    Value& getApplyEQ() {return applyEQ;}
    EQFilters* getEqFilters() {return &eqFilters;}
    
    SoundTouchProcessor::PlaybackSettings getSoundTouchSettings (){return soundTouchSettings;}
    
private:
    AudioDeviceManager audioDeviceManager;	// this wraps the actual audio device
	AudioSourcePlayer audioSourcePlayer;
	AudioFormatManager formatManager;
    
    TimeSliceThread transportThread;
	AudioTransportSource transport;
	
	ScopedPointer<AudioFormatReaderSource> currentAudioFileSource;
	EQFilters eqFilters;
	ScopedPointer<SoundTouchAudioSource> soundTouch;
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