/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent()
{
	checkFirstTime();
    
    addAndMakeVisible(&guiControl);
	guiControl.setAudioControl(&audioControl);
	
//    addAndMakeVisible(&coverFlowComponent);

	ITunesLibrary::getInstance()->setLibraryTree (singletonLibraryTree);

	musicTable.setLibraryToUse (ITunesLibrary::getInstance());
	musicTable.addActionListener(this);
	addAndMakeVisible(&musicTable);
    

	setSize (1000, 930);
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::resized()
{
	guiControl.setBounds(0, 0, getWidth(),400);
	musicTable.setBounds(0, getHeight()/2, getWidth(), getHeight()/2);
//    coverFlowComponent.setBounds(0, 0, 100, 100);

}


//MenuBarCallbacks==============================================================
StringArray MainContentComponent::getMenuBarNames()
{
	const char* const names[] = { "File", 0 };
	return StringArray (names);
}

//static void alertBoxResultChosen (int result)
//{
//	AlertWindow::showMessageBoxAsync (AlertWindow::InfoIcon,
//									  "Alert Box",
//									  "Result code: " + String (result));
//}


PopupMenu MainContentComponent::getMenuForIndex (int topLevelMenuIndex, const String& menuName)
{
	PopupMenu fileMenu;
	if (topLevelMenuIndex == 0)
	{
		fileMenu.addItem(OpenFile, "Open Files", true, false);
		fileMenu.addItem(OpenDirectory, "Open Directory", true, false);
		fileMenu.addSeparator();
        fileMenu.addItem(AudioPrefs, "Audio Preferences", true, false);
		fileMenu.addItem(ImportItunes, "Import Itunes Library", true, false);
	}
	return fileMenu;
}

void MainContentComponent::menuItemSelected (int menuItemID, int topLevelMenuIndex)
{
	if (topLevelMenuIndex == FileMenu) 
    {
        if (menuItemID == AudioPrefs) 
        {
            audioControl.showAudioPreferences(this);
        }
        if (menuItemID == OpenFile)
		{
			File audioFile;
			
			FileChooser fc ("Choose a file to open...",File::getCurrentWorkingDirectory(),"*",true);
			
			if (fc.browseForMultipleFilesToOpen())
			{
				String fileChosen;
				for (int i = 0; i < fc.getResults().size(); ++i)
				{
					fileChosen << fc.getResults().getReference(i).getFullPathName() << "\n";
				}

				audioFile = fc.getResult();
				audioControl.loadFile(audioFile);
				guiControl.updateTagDisplay(audioFile);
			}
		}
		
		if (menuItemID == OpenDirectory)
		{
			FileChooser fc ("Choose a directory to open...",File::getCurrentWorkingDirectory(),"*",true);
			if (fc.browseForDirectory()) 
			{
				String initialDirectory;
				String filesFound;
				String filesInfo;
				
				
				initialDirectory = fc.getResult().getFullPathName();
				
				DirectoryIterator directoryIterator (initialDirectory, true, "*.mp3",2);
				
				while (directoryIterator.next())
				{
					File fileFound (directoryIterator.getFile());
					std::cout << "loading \n";
					filesFound<<fileFound.getParentDirectory().getFileName()<<"/"<<fileFound.getFileName()<<"\n";
				}
				filesInfo << "You picked: " << initialDirectory << "\n" << "Files: \n" << filesFound;   
				AlertWindow::showMessageBoxAsync (AlertWindow::InfoIcon, "Directory Chooser...", filesInfo);
			}		
		}
		
		if (menuItemID == ImportItunes)
		{
			if(AlertWindow::showOkCancelBox (AlertWindow::WarningIcon,
											 "Itunes Import",
											 "Importing Itunes library will delete current Library",
											 "Import Itunes Library",
											 "Keep Current Library",
											 0))
			{
				ITunesLibrary::getInstance()->setLibraryFile (ITunesLibrary::getDefaultITunesLibraryFile());
			}
		}
		
    }
}


void MainContentComponent::actionListenerCallback (const String& message)
{
	if (message == "LibraryImportFinished") {
		DBG("library Loaded");
		ITunesLibrary::getInstance()->saveLibrary(singletonLibraryFile);
		singletonLibraryTree = ITunesLibrary::getInstance()->getLibraryTree();
		ITunesLibrary::getInstance()->setLibraryTree(singletonLibraryTree);
	}
}

void MainContentComponent::checkFirstTime()
{
    if(XmlHelpers::checkFirstTime())
    {
        if(XmlHelpers::createSettingsFile())
        {
            DBG("New settings file Created");  
        }
        else
        {
            DBG("Something has gone wrong with checkFirstTime as this means createSettings was called but settings already existed");
        }
    }
}