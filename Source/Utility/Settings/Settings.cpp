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
    if(checkFirstTime())
    {
        if(createSettingsFile())
        {
            DBG("New settings file Created"); 
        }
        else
        {
            DBG("Something has gone wrong with checkFirstTime as this means createSettings was called but settings already existed");
        }
    }
    initSettings();
}

Settings::~Settings()
{
	saveSingletons();
    clearSingletonInstance();
}

bool Settings::checkFirstTime()
{		
    File settingsFile (File::getSpecialLocation(File::userMusicDirectory).getFullPathName()+ "/MusicPlayer/Settings");
    
    if (!settingsFile.exists())
        return true;
    else
        return false;
}

bool Settings::createSettingsFile()
{
    File settingsFile (File::getSpecialLocation(File::userMusicDirectory).getFullPathName()+ "/MusicPlayer/Settings");
    
    if (!settingsFile.existsAsFile()) {
        ValueTree settingsValue ("SETTINGS");
        ValueTree libraryValue("LIBRARY");
        libraryValue.setProperty("LibraryFile", File(File::getSpecialLocation(File::userMusicDirectory).getFullPathName()+ "/MusicPlayer/MusicPlayerLibrary.xml").getFullPathName() ,0);
        
        libraryValue.setProperty("ValueID", "0", 0);
        libraryValue.setProperty("LibID", "0", 0);
        settingsValue.addChild(libraryValue, -1, 0);
        
        ValueTree playlistValue ("PLAYLIST");
        playlistValue.setProperty("PlaylistFile", File(File::getSpecialLocation(File::userMusicDirectory).getFullPathName()+ "/MusicPlayer/MusicPlayerPlaylists.xml").getFullPathName() ,0);
        settingsValue.addChild(playlistValue, -1, 0);
        
        ValueTree eqValue ("EQSETTINGS");
        eqValue.setProperty("EqSettingsFile", File(File::getSpecialLocation(File::userMusicDirectory).getFullPathName()+ "/MusicPlayer/MusicPlayerEQ.xml").getFullPathName() ,0);
        eqValue.setProperty("ID", 1, 0);
        eqValue.setProperty("On", 0, 0);
        settingsValue.addChild(eqValue, -1, 0);
        
        ScopedPointer<XmlElement> settingsCreateXml;
        settingsCreateXml = settingsValue.createXml();
        settingsCreateXml->writeToFile(settingsFile, String::empty);
        
        File library(File::getSpecialLocation (File::userMusicDirectory).getChildFile ("MusicPlayer/MusicPlayerPlaylists.xml"));
        File playlist(File::getSpecialLocation (File::userMusicDirectory).getChildFile ("MusicPlayer/MusicPlayerLibrary.xml"));
        
        //library.create();
        //playlist.create();
        
        settingsFile.create();
        return true;
    }
    return false;
}

void Settings::initSettings()
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
            
            currentLibId = library->getIntAttribute("LibID");
            currentValueTreeId = library->getIntAttribute("ValueID");
            DBG("Current Lib id = " << currentLibId);
            
			if (!libraryFile.existsAsFile())
			{
				libraryFile.create();
				DBG("FILE DOESNT EXIST");
			}
		}
		else {
			DBG("LibraryFile tag doesnt match");
		}
        
        XmlElement* playlist;
        playlist = settingsXml->getChildByName("PLAYLIST");
        if (playlist!= nullptr) {
            playlistFile = playlist->getStringAttribute("PlaylistFile");
            if (!playlistFile.existsAsFile())
            {
                playlistFile.create();
                DBG("Playlist file not found");
            }
        }
		
        XmlElement* eqXml;
        eqXml = settingsXml->getChildByName("EQSETTINGS");
        if (eqXml!= nullptr) {
            eqSettingsFile = eqXml->getStringAttribute("EqSettingsFile");
            eqDetails.ID = eqXml->getIntAttribute("ID");
            eqDetails.On = eqXml->getIntAttribute("On");
            if (!eqSettingsFile.existsAsFile())
            {
                eqSettingsFile.create();
                DBG("EQ file not found");
            }
        }
	}	
    
	libraryTree = readValueTreeFromFile(libraryFile);
    playlistTree = readValueTreeFromFile(playlistFile);
    eqSettings = readValueTreeFromFile(eqSettingsFile);
    
    if (!libraryTree.hasType(MusicColumns::libraryIdentifier)) {
        libraryTree = ValueTree(MusicColumns::libraryIdentifier);
        currentLibId = 0;
        currentValueTreeId = 0;
    }
    
    if (playlistTree.hasType(MusicColumns::playlistsIdentifier))
    {
        ValueTree lastEntry = playlistTree.getChild(playlistTree.getNumChildren()-1);
        currentPlaylistId = lastEntry.getProperty(MusicColumns::playlistsIdentifier);
    }
    else
    {
        playlistTree = ValueTree(MusicColumns::playlistsIdentifier);
        ValueTree library("ITEM");
        library.setProperty("Name", "Library", 0);
        library.setProperty("PlaylistID", 1, 0);
        library.setProperty("Size", 0, 0);
        
        playlistTree.addChild(library, -1, 0);
        currentPlaylistId = 0;
    }
    
    if (!eqSettings.hasType("EQSETTINGS"))
        eqSettings = ValueTree("EQSETTINGS");
    
    shouldPlay = false;
    
    undoManager = new UndoManager(30,30);
}

File& Settings::getLibraryFile()
{
	return libraryFile;
}

File& Settings::getPlaylistsFile()
{
    return playlistFile;
}

ValueTree& Settings::getLibraryTree()
{
	return libraryTree;
}

ValueTree& Settings::getPlaylistsTree()
{
    return playlistTree;
}

int& Settings::getCurrentLibId()
{
    return currentLibId;
}

int& Settings::getCurrentValueTreeId()
{
    return currentValueTreeId;
}

int& Settings::getCurrentPlaylistId()
{
    return currentPlaylistId;
}

Array<int>& Settings::getSelectedTracks()
{
    return selectedTracks;
}

OwnedArray <RemoteInterprocessConnection, CriticalSection>& Settings::getConnections()
{
    return connections;
}

ValueTree& Settings::getSelectedRow()
{
    return selectedRow;
}

ValueTree& Settings::getPlayingRow()
{
    return playingRow;
}

ValueTree& Settings::getFilteredList()
{
    return filteredDataTree;
}

Value& Settings::getShouldPlay()
{
    return shouldPlay;
}

Value& Settings::getPlayState()
{
    return playState;
}

Value& Settings::getUpdateRequired()
{
    return updateRequired;
}

Value& Settings::getTableDeleting()
{
    return deletingTable;
}

Value& Settings::getAlbumArtUpdate()
{
    return artUpdate;
}

ImageWithType& Settings::getArtClipboard()
{
    return albumArtClipboard;
}

UndoManager* Settings::getUndoManager()
{
    return undoManager;
}

void Settings::saveSingletons()
{
	writeValueTreeToFile(libraryTree, libraryFile);
    writeValueTreeToFile(playlistTree, playlistFile);
    writeValueTreeToFile(eqSettings, eqSettingsFile);
    
	ScopedPointer<XmlElement> settingsXml;
	
	settingsXml = XmlDocument::parse (settingsXmlFile);
	
	if (settingsXml != nullptr && settingsXml->hasTagName ("SETTINGS"))
	{
		XmlElement* library;
		library = settingsXml->getChildByName("LIBRARY");

		if (library != nullptr) {
			library->setAttribute("LibID", currentLibId);
            library->setAttribute("ValueID", currentValueTreeId);
		}
        
        XmlElement* eqDetailsXml;
        eqDetailsXml = settingsXml->getChildByName("EQSETTINGS");
        
        if (eqDetailsXml != nullptr)
        {
            eqDetailsXml->setAttribute("ID", eqDetails.ID);
            eqDetailsXml->setAttribute("On", eqDetails.On.toString());
        }
        
        settingsXml->writeToFile(settingsXmlFile, String::empty, "UTF-8", 200);
	}	

}
