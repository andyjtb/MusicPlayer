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
    
    if (libraryTree.hasType(MusicColumns::libraryIdentifier)) {
        ValueTree lastEntry = libraryTree.getChild(libraryTree.getNumChildren()-1);
        currentLibId = lastEntry.getProperty(MusicColumns::columnNames[MusicColumns::LibID]);
        currentValueTreeId = lastEntry.getProperty(MusicColumns::columnNames[MusicColumns::ID]);
        DBG("Current Lib id = " << currentLibId);
    }
    else
    {
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
//	ScopedPointer<XmlElement> settingsXml;
//	
//	settingsXml = XmlDocument::parse (settingsXmlFile);
//	
//	if (settingsXml != nullptr && settingsXml->hasTagName ("SETTINGS"))
//	{
//		XmlElement* library;
//		library = settingsXml->getChildByName("LIBRARY");
//
//		if (library != nullptr) {
//            ValueTree lastEntry = libraryTree.getChild(libraryTree.getNumChildren()-1);
//            
//			XmlElement* libId = library->getChildByName("LibID");
//            libId->setText(lastEntry.getProperty(MusicColumns::columnNames[MusicColumns::LibID]));
//            
//            XmlElement* valueTreeId = library->getChildByName("ValueId");
//            valueTreeId->setText(lastEntry.getProperty(MusicColumns::columnNames[MusicColumns::LibID]));
//            
//		}
//        
//        settingsXml->writeToFile(settingsXmlFile, String::empty, "UTF-8", 200);
//	}	

}
