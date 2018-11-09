/*
  ==============================================================================
  
  This file is part of the dRowAudio JUCE module
  Copyright 2004-12 by dRowAudio.
  
  ------------------------------------------------------------------------------
 
  dRowAudio can be redistributed and/or modified under the terms of the GNU General
  Public License (Version 2), as published by the Free Software Foundation.
  A copy of the license is included in the module distribution, or can be found
  online at www.gnu.org/licenses.
  
  dRowAudio is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
  A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
  
  ==============================================================================
*/

#include "ITunesLibrary.h"

juce_ImplementSingleton(ITunesLibrary);

ITunesLibrary::ITunesLibrary()
    : libraryTree (MusicColumns::libraryIdentifier)
{
}

ITunesLibrary::~ITunesLibrary()
{
    clearSingletonInstance();
}

void ITunesLibrary::setLibraryFile (File newFile)
{
	if (newFile.existsAsFile()) 
	{
		listeners.call (&Listener::libraryChanged, this);
		parser = new ITunesLibraryParser (newFile, libraryTree, playlistsTree, parserLock);
		startTimer(500);
	}	
}

//==============================================================================
const File ITunesLibrary::getDefaultITunesLibraryFile()
{
    File iTunesLibrary(File::getSpecialLocation (File::userMusicDirectory).getChildFile ("iTunes/iTunes Library.xml"));
    //Non drow
    if (!iTunesLibrary.existsAsFile())
    {
        iTunesLibrary = (File::getSpecialLocation (File::userMusicDirectory).getChildFile ("iTunes/iTunes Music Library.xml"));
        
        if (!iTunesLibrary.existsAsFile())
            {
                iTunesLibrary = libraryNotFound();
            }
    }
    
    return iTunesLibrary;
}

//==============================================================================
void ITunesLibrary::setLibraryTree (ValueTree& newTreeToUse)
{
    if (! newTreeToUse.isValid()) 
    {
        newTreeToUse = ValueTree (MusicColumns::libraryIdentifier);
    }

    libraryTree = newTreeToUse;
    libraryTree.addListener(this);
}

void ITunesLibrary::setPlaylistsTree (ValueTree& newPlaylistTreeToUse)
{
    if (! newPlaylistTreeToUse.isValid())
    {
        newPlaylistTreeToUse = ValueTree (MusicColumns::playlistsIdentifier);
    }
    
    playlistsTree = newPlaylistTreeToUse;
    playlistsTree.addListener(this);
}

void ITunesLibrary::timerCallback()
{
	if (parser != nullptr)
	{
		listeners.call (&Listener::libraryUpdated, this);

		if (parser->hasFinished())
        {
			stopTimer();
			parser = nullptr;
			listeners.call (&Listener::libraryFinished, this);
		}
	}
}

//==============================================================================
void ITunesLibrary::addListener (ITunesLibrary::Listener* const listener)
{
    listeners.add (listener);
}

void ITunesLibrary::removeListener (ITunesLibrary::Listener* const listener)
{
    listeners.remove (listener);
}

//==============================================================================
//NON DROW FUNCTIONS
void ITunesLibrary::saveLibrary()
{
//	File location(File::getSpecialLocation(File::userHomeDirectory));
//	location = (location.getFullPathName()+"/test.xml");
    singletonLibraryFile.create();
    singletonPlaylistsFile.create();
    
	bool success = writeValueTreeToFile(libraryTree, singletonLibraryFile, true);
    bool playlistSuccess = writeValueTreeToFile(playlistsTree, singletonPlaylistsFile);
    DBG("Did Library save = " << (success ? "1" : "0"));
    DBG("Did Playlists save = " << (playlistSuccess ? "1" : "0"));
}

File ITunesLibrary::libraryNotFound()
{
    File selectedFile;
    
    AlertWindow itunesPopup("Locate iTunes Library", "iTunes Library could not be found \n Please find your iTunes Library File: ", AlertWindow::WarningIcon);
    FilenameComponent libraryName("Library File:", 
                                  File::getSpecialLocation(File::userMusicDirectory).getChildFile("iTunes"),
                                  true, false, false, "*.xml", "",
                                  "");
    
    libraryName.setBounds(0,0,375,30);
    
    itunesPopup.addCustomComponent(&libraryName);
    
    itunesPopup.addButton("Cancel", 0);
    itunesPopup.addButton("Ok", 1);
    
    if (itunesPopup.runModalLoop() != 0) {
        ScopedPointer<XmlElement> iTunesDatabase(XmlDocument::parse (libraryName.getCurrentFile()));
        if (! iTunesDatabase->hasTagName ("plist")
            || iTunesDatabase->getStringAttribute ("version") != "1.0")
        {
            AlertWindow invalidWindow("Locate iTunes Library", "Please select a valid iTunes Library File", AlertWindow::WarningIcon);
            invalidWindow.addButton("Ok", 0);
            if (invalidWindow.runModalLoop() == 0)
            {}
        }
        selectedFile = libraryName.getCurrentFile();
    }
    
    itunesPopup.removeCustomComponent(0); 
    return selectedFile;
}

void ITunesLibrary::valueTreePropertyChanged (ValueTree &treeWhosePropertyHasChanged, const Identifier &property)
{
    listeners.call (&Listener::libraryUpdated, this);
}
void ITunesLibrary::valueTreeChildAdded (ValueTree &parentTree, ValueTree &childWhichHasBeenAdded)
{
    listeners.call (&Listener::libraryUpdated, this);
}
void ITunesLibrary::valueTreeChildRemoved (ValueTree &parentTree, ValueTree &childWhichHasBeenRemoved, int)
{
    listeners.call (&Listener::libraryUpdated, this);
}
void ITunesLibrary::valueTreeChildOrderChanged (ValueTree &parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex)
{}
void ITunesLibrary::valueTreeParentChanged (ValueTree &treeWhoseParentHasChanged)
{}
