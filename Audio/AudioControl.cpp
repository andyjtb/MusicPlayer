/*
 *  AudioControl.cpp
 *  sdaMidiMeter
 *
 *  Created by Andy Brown on 12/8/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "AudioControl.h"



AudioControl::AudioControl()
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
//	soundTouch = new SoundTouchAudioSource(&transport, false);
//        audioSourcePlayer.setSource (soundTouchSource);

		audioSourcePlayer.setSource(&transport);
		
        
        // start the IO device pulling its data from our callback..
        audioDeviceManager.addAudioCallback (this);
    }
	
	audioMeterOutputL = audioMeterOutputR = sharedMeterOutputL = sharedMeterOutputR = 0.f;
	
}
AudioControl::~AudioControl()
{
    
    audioSourcePlayer.setSource (nullptr);
	transport.setSource(nullptr);//unload the current file
    audioDeviceManager.removeAudioCallback(this);

}

void AudioControl::loadFile (const File audioFile)
{
	//this is called when the user changes the filename in the file chooser box
	if(audioFile.existsAsFile())
	{
		
		// unload the previous file source and delete it..
		transport.stop();
		transport.setSource (nullptr);
		currentAudioFileSource = nullptr;

		
		// create a new file source from the file..
		// get a format manager and set it up with the basic types (wav, ogg and aiff).
		AudioFormatManager formatManager;
		formatManager.registerBasicFormats();
		
		AudioFormatReader* reader = formatManager.createReaderFor (audioFile);
		
		if (reader != nullptr)
		{
			//currentFile = audioFile;
			currentAudioFileSource = new AudioFormatReaderSource (reader, true);
			
			// ..and plug it into our transport source
			transport.setSource (currentAudioFileSource);
			
			sendChangeMessage();
			
		}  
	}
	else
	{
		AlertWindow::showMessageBox (AlertWindow::WarningIcon,
									 ProjectInfo::projectName,
									 "Audio File could not be opened/found!\n\n");
	}	    
}

bool AudioControl::isPlaying()
{
	return transport.isPlaying();
}

void AudioControl::setVolume(double incomingVolume)
{
	audioSourcePlayer.setGain(incomingVolume);
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

//void AudioControl::setPlaybackSpeed(const float incomingSpeed)
//{	
//    if (incomingSpeed >= 0.f && incomingSpeed <= 10.f) 
//    {
//		soundTouchSettings.tempo = incomingSpeed;
//        soundtouchAudioSource->setPlaybackSettings(soundTouchSettings);
//    }
//}
//
//void AudioControl::setPitch(const float incomingPitch)
//{
//	if (incomingPitch >= 0.f && incomingPitch <= 10.f) 
//    {
//		soundTouchSettings.pitch = incomingPitch;
//        soundtouchAudioSource->setPlaybackSettings(soundTouchSettings);
//    }
//}


//AudioCallbacks================================================================
void AudioControl::audioDeviceIOCallback (const float** inputChannelData,
										  int numInputChannels,
										  float** outputChannelData,
										  int numOutputChannels,
										  int numSamples)
{    
	const float *inL;
	float *outL;
	float *outR;
	
	audioSourcePlayer.audioDeviceIOCallback(inputChannelData, numInputChannels, outputChannelData, numOutputChannels, numSamples);
    
	inL = inputChannelData[0];
	outL = outputChannelData[0]; 
    outR = outputChannelData[1];
	
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
}


void AudioControl::audioDeviceAboutToStart (AudioIODevice* device)
{
	audioSourcePlayer.audioDeviceAboutToStart (device);
}

void AudioControl::audioDeviceStopped()
{
	audioSourcePlayer.audioDeviceStopped();
}

float AudioControl::getOutputMeterValue(const char* channel)
{
	if (channel == "L") {
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
    DialogWindow::showModalDialog ("Audio Settings", &audioSettingsComp, centerComponent, Colours::azure, true);
    
}

void AudioControl::valueChanged (Value& valueChanged)
{
    if (valueChanged == singletonPlayState)
    {
        singletonPlayState.getValue() ? transport.start() : transport.stop();
    }
}