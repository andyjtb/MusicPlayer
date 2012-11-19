/*
 *  LayoutValueTree.cpp
 *  LoopStation
 *
 *  Created by Andy on 01/03/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "Settings.h"

juce_ImplementSingleton_SingleThreaded(Settings)

Settings::Settings() 
{
	settingsXmlFile = (File::getSpecialLocation(File::userMusicDirectory).getFullPathName() + "/MusicPlayer/Settings");

	ScopedPointer<XmlElement> settingsXml;
	
	settingsXml = XmlDocument::parse (settingsXmlFile);
	
	if (settingsXml != nullptr && settingsXml->hasTagName ("SETTINGS"))
	{        
        XmlElement* library;
		library = settingsXml->getChildByName("LIBRARY");
		if (library != nullptr) {
			libraryFile  = library->getStringAttribute("LibraryFile");
			if (!libraryFile.existsAsFile())
			{
				libraryFile.create();
				DBG("FILE DOESNT EXIST");
			}
		}
		else {
			DBG("LibraryFile tag doesnt match");
		}
		
	}	

	libraryTree = readValueTreeFromFile(libraryFile);
    
    ValueTree lastEntry = libraryTree.getChild(libraryTree.getNumChildren()-1);
	currentLibId = lastEntry.getProperty(MusicColumns::columnNames[MusicColumns::LibID]);
    currentValueTreeId = lastEntry.getProperty(MusicColumns::columnNames[MusicColumns::ID]);
    DBG("Current Lib id = " << currentLibId);
}

Settings::~Settings()
{
//	saveSingletons();
    clearSingletonInstance();
}

File& Settings::getLibraryFile()
{
	return libraryFile;
}

ValueTree& Settings::getLibraryTree()
{
	return libraryTree;
}

int& Settings::getCurrentLibId()
{
    return currentLibId;
}

int& Settings::getCurrentValueTreeId()
{
    return currentValueTreeId;
}

void Settings::saveSingletons()
{	
	ScopedPointer<XmlElement> settingsXml;
	
	settingsXml = XmlDocument::parse (settingsXmlFile);
	
	if (settingsXml != nullptr && settingsXml->hasTagName ("SETTINGS"))
	{
		XmlElement* library;
		library = settingsXml->getChildByName("LIBRARY");

		if (library != nullptr) {
            ValueTree lastEntry = libraryTree.getChild(libraryTree.getNumChildren()-1);
            
			XmlElement* libId = library->getChildByName("LibID");
            libId->setText(lastEntry.getProperty(MusicColumns::columnNames[MusicColumns::LibID]));
            
            XmlElement* valueTreeId = library->getChildByName("ValueId");
            valueTreeId->setText(lastEntry.getProperty(MusicColumns::columnNames[MusicColumns::LibID]));
            
		}
        
        settingsXml->writeToFile(settingsXmlFile, String::empty, "UTF-8", 200);
	}	

}
