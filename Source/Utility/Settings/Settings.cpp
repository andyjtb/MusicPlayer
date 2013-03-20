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
		
	}	

	libraryTree = readValueTreeFromFile(libraryFile);
    playlistTree = readValueTreeFromFile(playlistFile);
    
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
    
    shouldPlay = false;
    
    undoManager = new UndoManager(30,30);
}

Settings::~Settings()
{
	saveSingletons();
    clearSingletonInstance();
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

Value& Settings::getLoadSelected()
{
    return loadSelected;
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
        
        settingsXml->writeToFile(settingsXmlFile, String::empty, "UTF-8", 200);
	}	

}
