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
        settingsFile.getParentDirectory().createDirectory();
        
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
        
        ValueTree tableLayout ("TABLELAYOUT");
        tableLayout.setProperty("Layout", "<TABLELAYOUT sortedCol=\"3\" sortForwards=\"1\"><COLUMN id=\"1\" visible=\"0\" width=\"30\"/><COLUMN id=\"2\" visible=\"0\" width=\"50\"/><COLUMN id=\"3\" visible=\"1\" width=\"150\"/><COLUMN id=\"4\" visible=\"1\" width=\"200\"/><COLUMN id=\"12\" visible=\"1\" width=\"50\"/><COLUMN id=\"5\" visible=\"1\" width=\"150\"/><COLUMN id=\"7\" visible=\"0\" width=\"60\"/><COLUMN id=\"9\" visible=\"0\" width=\"100\"/><COLUMN id=\"10\" visible=\"0\" width=\"100\"/><COLUMN id=\"11\" visible=\"0\" width=\"30\"/><COLUMN id=\"13\" visible=\"0\" width=\"60\"/><COLUMN id=\"14\" visible=\"1\" width=\"125\"/><COLUMN id=\"15\" visible=\"0\" width=\"125\"/><COLUMN id=\"16\" visible=\"0\" width=\"300\"/><COLUMN id=\"17\" visible=\"0\" width=\"60\"/><COLUMN id=\"22\" visible=\"1\" width=\"50\"/><COLUMN id=\"18\" visible=\"1\" width=\"50\"/><COLUMN id=\"19\" visible=\"0\" width=\"80\"/><COLUMN id=\"20\" visible=\"0\" width=\"50\"/><COLUMN id=\"21\" visible=\"0\" width=\"50\"/><COLUMN id=\"8\" visible=\"1\" width=\"80\"/><COLUMN id=\"6\" visible=\"1\" width=\"60\"/></TABLELAYOUT>", 0);
        settingsValue.addChild(tableLayout, -1, 0);
        
        ValueTree lastFM ("LASTFM");
        lastFM.setProperty("Enabled", 0, 0);
        lastFM.setProperty("SessionKey", String::empty, 0);
        lastFM.setProperty("UserName", String::empty, 0);
        settingsValue.addChild(lastFM, -1, 0);
        
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
        
        XmlElement* tableLayoutXml;
        tableLayoutXml = settingsXml->getChildByName("TABLELAYOUT");
        if (tableLayoutXml != nullptr) {
            tableLayout = tableLayoutXml->getStringAttribute("Layout");
        }
        
        XmlElement* lastFmXml;
        lastFmXml = settingsXml->getChildByName("LASTFM");
        if (lastFmXml != nullptr) {
            lastFm.getEnabled().setValue(lastFmXml->getBoolAttribute("Enabled"));
            lastFm.getSessionKey() = lastFmXml->getStringAttribute("SessionKey", String::empty);
            lastFm.getUserName() = lastFmXml->getStringAttribute("UserName", String::empty);
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
    
    //Restore eq state
    if (eqSettings.isValid())
    {
        //Minus 2 as in the dropdown box where it is set, 0 is not allowed and 1 is always flat
        ValueTree currentEqPreset = eqSettings.getChild(eqDetails.ID - 2);
        if (currentEqPreset.isValid())
            for (int i = 1; i < currentEqPreset.getNumProperties(); i++)
                eqDetails.gains.add(float(currentEqPreset.getProperty(currentEqPreset.getPropertyName(i))));

    }
    
    
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
        
        XmlElement* tableLayoutXml;
        tableLayoutXml = settingsXml->getChildByName("TABLELAYOUT");
        
        if (tableLayoutXml != nullptr)
            tableLayoutXml->setAttribute("Layout", tableLayout);
        
        XmlElement* lastFmXml;
        lastFmXml = settingsXml->getChildByName("LASTFM");
        
        if (lastFmXml != nullptr)
        {
            lastFmXml->setAttribute("Enabled", bool(lastFm.getEnabled().getValue()));
            lastFmXml->setAttribute("SessionKey", lastFm.getSessionKey());
            lastFmXml->setAttribute("UserName", lastFm.getUserName());
        }
        
        settingsXml->writeToFile(settingsXmlFile, String::empty, "UTF-8", 200);
	}	

}
