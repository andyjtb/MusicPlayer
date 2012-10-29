/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent()
{
	addAndMakeVisible(&guiControl);
	guiControl.setAudioControl(&audioControl);

	ITunesLibrary::getInstance()->setLibraryFile (ITunesLibrary::getDefaultITunesLibraryFile());
	musicTable.setLibraryToUse (ITunesLibrary::getInstance());
	addAndMakeVisible(&musicTable);
    

	setSize (1000, 930);
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::resized()
{
	guiControl.setBounds(0, 0, 500,400);
	musicTable.setBounds(0, getHeight()/2, getWidth()/2, getHeight()/2);

}


//MenuBarCallbacks==============================================================
StringArray MainContentComponent::getMenuBarNames()
{
	const char* const names[] = { "File", 0 };
	return StringArray (names);
}

PopupMenu MainContentComponent::getMenuForIndex (int topLevelMenuIndex, const String& menuName)
{
	PopupMenu fileMenu;
	if (topLevelMenuIndex == 0)
	{
		fileMenu.addItem(OpenFile, "Open Files", true, false);
		fileMenu.addItem(OpenDirectory, "Open Directory", true, false);
		fileMenu.addSeparator();
        fileMenu.addItem(AudioPrefs, "Audio Preferences", true, false);
		fileMenu.addItem(testSave, "Save iTunes Lib Test", true, false);
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
				
				//AlertWindow::showMessageBoxAsync (AlertWindow::InfoIcon,
				//												  "File Chooser...",
				//												  "You picked: " + fileChosen);
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
		
		if (menuItemID == testSave) {
			ITunesLibrary::getInstance()->saveLibrary();
		}

    }
}