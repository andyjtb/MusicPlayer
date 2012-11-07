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
	File settingsXmlFile(File::getSpecialLocation(File::currentApplicationFile).getFullPathName() + "/Contents/Resources/Settings.xml");
	
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
}

Settings::~Settings()
{
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


