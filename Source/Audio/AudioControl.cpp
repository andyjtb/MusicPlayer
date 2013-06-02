/*
 *  AudioControl.cpp
 *  sdaMidiMeter
 *
 *  Created by Andy Brown on 12/8/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "AudioControl.h"



AudioControl::AudioControl() : bufferingThread("MusicPlayer AudioBuffer")
{	
	singletonPlayState.addListener(this);
    
    const String error (audioDeviceManager.initialise (0, /* number of input channels */
													   2, /* number of output channels */
													   0, /* no XML settings.. */
													   true  /* select default device on failure */));	
    if (error.isNotEmpty())
    {
        AlertWindow::showMessageBox (AlertWindow::WarningIcon,
                                     ProjectInfo::projectName,
                                     "Couldn't open an output device!\n\n" + error);
    }
    else
    {
		audioSourcePlayer.setSource(&transport);
    
        // start the IO device pulling its data from our callback..
        audioDeviceManager.addAudioCallback (this);
    }
    
    // Sets format manager to be able to read wav aiff flac ogg and OS specific (aac m4a wmv) etc
    formatManager.registerBasicFormats();
    DBG(formatManager.getWildcardForAllFormats());

	audioMeterOutputL = audioMeterOutputR = sharedMeterOutputL = sharedMeterOutputR = 0.f;
    
    bufferingThread.startThread();
    
    applyEQ.referTo(currentEqDetails.On);
    
}
AudioControl::~AudioControl()
{
    bufferingThread.stopThread(100);
    audioSourcePlayer.setSource (nullptr);
	transport.setSource(nullptr);//unload the current file
    soundTouch = nullptr;
    bufferingAudioSource = nullptr;
    
    audioDeviceManager.removeAudioCallback(this);

}

int AudioControl::loadFile (const File& audioFile)
{
	//this is called when the user changes the filename in the file chooser box
	if(audioFile.existsAsFile())
	{
        if (soundTouch != nullptr)
            soundTouchSettings = soundTouch->getPlaybackSettings();
        
//        if (bufferingThread.isThreadRunning()) {
//            DBG("Thread is running, doesn't call notify");
//        }
//        else
//            bufferingThread.notify();
        
		AudioFormatReader* reader = formatManager.createReaderFor (audioFile);
		
		if (reader != nullptr)
		{
            // unload the previous file source and delete it..
            transport.stop();
            transport.setSource (nullptr);
            soundTouch = nullptr;
            bufferingAudioSource = nullptr;
            
            eqFilters.setSampleRate(reader->sampleRate);
			currentAudioFileSource = new AudioFormatReaderSource (reader, true);
            
			// ..and plug it into our effects sources
            bufferingAudioSource = new BufferingAudioSource (currentAudioFileSource, bufferingThread, false, 88200);
            soundTouch = new SoundTouchAudioSource (bufferingAudioSource);
            soundTouch->setPlaybackSettings(soundTouchSettings);

            transport.setSource (soundTouch);

			sendChangeMessage();
            
            //Loaded fine
            return 0 ;
		}  
        else
        {
            //Reader failed
            return 2;
        }
	}
	else
	{
        //Not found
        return 1;
	}	    
}

//bool AudioControl::isPlaying()
//{
//	return transport.isPlaying();
//}

void AudioControl::setVolume(double incomingVolume)
{
	audioSourcePlayer.setGain(incomingVolume);
}

double AudioControl::getVolume()
{
    return audioSourcePlayer.getGain();
}

double AudioControl::getTransportPosition()
{
	return transport.getCurrentPosition();
}

void AudioControl::setTransportPosition(double position)
{
	transport.setPosition(position);
	
}

double AudioControl::getTransportLength()
{
	return transport.getLengthInSeconds();
}

void AudioControl::setPlaybackSpeed(const float incomingSpeed)
{	
    if (incomingSpeed >= 0.f && incomingSpeed <= 10.f) 
    {
		soundTouchSettings.tempo = incomingSpeed;
        if (soundTouch != nullptr)
            soundTouch->setPlaybackSettings(soundTouchSettings);
    }
}

void AudioControl::setPitch(const float incomingPitch)
{
	if (incomingPitch >= 0.f && incomingPitch <= 10.f) 
    {
		soundTouchSettings.pitch = incomingPitch;
        if (soundTouch != nullptr)
            soundTouch->setPlaybackSettings(soundTouchSettings);
    }
}


//AudioCallbacks================================================================
void AudioControl::audioDeviceIOCallback (const float** inputChannelData,
										  int numInputChannels,
										  float** outputChannelData,
										  int numOutputChannels,
										  int numSamples)
{   
    
    float *outL;
    float *outR;
    
    outL = outputChannelData[0]; 
    outR = outputChannelData[1];
    //if (isPlaying())
    //{ 
        audioSourcePlayer.audioDeviceIOCallback(inputChannelData, numInputChannels, outputChannelData, numOutputChannels, numSamples);
        
        if (applyEQ.getValue())
        {
            eqFilters.applyFilters (outputChannelData, numSamples, numOutputChannels);
        }
        
        
        while(numSamples--) 
        {		
            if (fabsf(*outL) > audioMeterOutputL) 
                audioMeterOutputL = fabsf(*outL);
            else 
                audioMeterOutputL *= 0.9999;
            
            if (fabsf(*outR) > audioMeterOutputR) 
                audioMeterOutputR = fabsf(*outR);
            else 
                audioMeterOutputR *= 0.9999;
            
            outL++; 
            outR++;	
            
        }
        
        sharedMemory.enter();
        sharedMeterOutputL = audioMeterOutputL;
        sharedMeterOutputR = audioMeterOutputR;
        sharedMemory.exit();
//    }
    
//    else
//    {
//        zeromem (outL, sizeof (float) * numSamples);
//        zeromem (outR, sizeof (float) * numSamples);
//        
//        outputMeterAnimatedToZero();
//    }
}


void AudioControl::audioDeviceAboutToStart (AudioIODevice* device)
{
	audioSourcePlayer.audioDeviceAboutToStart (device);
}

void AudioControl::audioDeviceStopped()
{
	audioSourcePlayer.audioDeviceStopped();
}

void AudioControl::outputMeterAnimatedToZero()
{
    int i = 300;
    while (i--)
    {
        sharedMemory.enter();
        sharedMeterOutputL *= 0.9999;
        sharedMeterOutputR *= 0.9999;
        sharedMemory.exit(); 
    }
}

float AudioControl::getOutputMeterValue(const char* channel)
{
	if (strcmp(channel, "L") == 0) {
		float valueL;
		
		sharedMemory.enter();
		valueL = sharedMeterOutputL;
		sharedMemory.exit();
		
		return valueL;
	}
	
	else {
		float valueR;
		
		sharedMemory.enter();
		valueR = sharedMeterOutputR;
		sharedMemory.exit();
		
		return valueR;
	}
	
}

void AudioControl::showAudioPreferences(Component* centerComponent)
{
    AudioDeviceSelectorComponent audioSettingsComp (audioDeviceManager,
                                                    0, 2, 2, 2,
                                                    true, true, true, true);
    audioSettingsComp.setSize (500, 250);
    DialogWindow::showModalDialog ("Audio Settings", &audioSettingsComp, centerComponent, Colours::lightgrey, true);
    
}

void AudioControl::valueChanged (Value& valueChanged)
{
    if (valueChanged == singletonPlayState)
    {
        //singletonPlayState.getValue() ? transport.start() : transport.stop();
        if (singletonPlayState.getValue())
        {
            //bufferingThread.notify();
            transport.start();
        }
        else
        {
            transport.stop();

            //Wait indefinately, quicker and less processor intensive than stopping the thread
            //if(bufferingThread.isThreadRunning())
                //bufferingThread.wait(-1);
        }
    }
}