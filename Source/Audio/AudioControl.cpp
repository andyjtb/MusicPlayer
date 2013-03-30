/*
 *  AudioControl.cpp
 *  sdaMidiMeter
 *
 *  Created by Andy Brown on 12/8/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "AudioControl.h"



AudioControl::AudioControl() : transportThread("MusicPlayer AudioBuffer")
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
//        soundTouch = new SoundTouchAudioSource(&transport, false);
//        audioSourcePlayer.setSource (soundTouch);
		audioSourcePlayer.setSource(&transport);
    
        // start the IO device pulling its data from our callback..
        audioDeviceManager.addAudioCallback (this);
    }
    
    // Sets format manager to be able to read wav aiff flac ogg and OS specific (aac m4a wmv) etc
    formatManager.registerBasicFormats();

	audioMeterOutputL = audioMeterOutputR = sharedMeterOutputL = sharedMeterOutputR = 0.f;
    
	transportThread.startThread();
}
AudioControl::~AudioControl()
{
    transportThread.stopThread(100);
    audioSourcePlayer.setSource (nullptr);
	transport.setSource(nullptr);//unload the current file
    audioDeviceManager.removeAudioCallback(this);

}

void AudioControl::loadFile (const File& audioFile)
{
	//this is called when the user changes the filename in the file chooser box
	if(audioFile.existsAsFile())
	{
		// unload the previous file source and delete it..
		transport.stop();
		transport.setSource (nullptr);
		currentAudioFileSource = nullptr;
		
		AudioFormatReader* reader = formatManager.createReaderFor (audioFile);
		
		if (reader != nullptr)
		{
            eqFilters.setSampleRate(reader->sampleRate);
			currentAudioFileSource = new AudioFormatReaderSource (reader, true);
            
			// ..and plug it into our transport source
            soundTouch.setSource(currentAudioFileSource);
			transport.setSource (&soundTouch, 88200, &transportThread);

			sendChangeMessage();
		}  
        else
        {
            DBG("Reader failed\nFile is = "<< audioFile.getFullPathName());
            //reloadFile(audioFile, true);
        }
	}
	else
	{
        DBG("File doesn't exist");
        //reloadFile(audioFile, false);
	}	    
}

void AudioControl::reloadFile(const File& audioFile, bool couldBeFound)
{
    ScopedPointer<AlertWindow> fileFailed;
    if (couldBeFound) {
        fileFailed = new AlertWindow("File Could Not Be Opened", "The selected file could not be opened, the listing for it could be incorrect\n Would you like to try reloading this file?", AlertWindow::WarningIcon);
        fileFailed->addButton("Reload", 1);
    }
    else
    {
        fileFailed = new AlertWindow("File Could Not Be Found", "The selected file could not be found, the listing for it could be incorrect\n Would you like to try finding this file?", AlertWindow::WarningIcon);
        fileFailed->addButton("Find", 1);
    }
    
    fileFailed->addButton("Cancel", 0);
    
    
    if (fileFailed->runModalLoop() != 0)
    {
        ValueTree currentTrack = filteredDataList.getChildWithProperty(MusicColumns::columnNames[MusicColumns::Location], audioFile.getFullPathName());
        if (currentTrack.isValid())
        {
            singletonLibraryTree.removeChild(currentTrack, 0/*add undoManager possibly*/);
            
            if (couldBeFound) {
                ValueTree reloadChild = TagReader::addToLibrary(audioFile);
                if (reloadChild.isValid()) 
                    singletonLibraryTree.addChild(reloadChild, -1, 0);
            }
            else
            {
                
            }
            tableUpdateRequired = true;
        }
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
        soundTouch.setPlaybackSettings(soundTouchSettings);
    }
}

void AudioControl::setPitch(const float incomingPitch)
{
	if (incomingPitch >= 0.f && incomingPitch <= 10.f) 
    {
		soundTouchSettings.pitch = incomingPitch;
        soundTouch.setPlaybackSettings(soundTouchSettings);
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
    DialogWindow::showModalDialog ("Audio Settings", &audioSettingsComp, centerComponent, Colours::lightgrey, true);
    
}

void AudioControl::valueChanged (Value& valueChanged)
{
    if (valueChanged == singletonPlayState)
    {
        singletonPlayState.getValue() ? transport.start() : transport.stop();
    }
}