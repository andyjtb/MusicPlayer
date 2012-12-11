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
#include "Settings.h"
#include "dRowAudio_SoundTouchAudioSource.h"


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
    void loadFile (const File audioFile);
    
    /**
     Sets the play state
     */
//    void setPlayState(bool shouldPlay);
    
    /**
     Toggles the current play state
     */
//    void togglePlayState();
    
    bool isPlaying();

//	void setPitch (const float incomingPitch);
//	
//	void setPlaybackSpeed(const float incomingSpeed);
	
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
	
	double getTransportPosition();
	void setTransportPosition(double position);
	double getTransportLength();
	
	void valueChanged (Value& valueChanged);
    
private:
    AudioDeviceManager audioDeviceManager;	// this wraps the actual audio device
	AudioSourcePlayer audioSourcePlayer;
	
	AudioTransportSource transport;
	
	ScopedPointer<AudioFormatReaderSource> currentAudioFileSource;
	
	//SoundTouchAudioSource* soundTouch;
	//SoundTouchProcessor::PlaybackSettings soundTouchSettings;
	
	//audio variables
    float audioMeterOutputL;
	float audioMeterOutputR;
    
    //Shared Memory Lock
    CriticalSection sharedMemory;
    float sharedMeterOutputL;
	float sharedMeterOutputR;
};



#endif //H_AUDIO