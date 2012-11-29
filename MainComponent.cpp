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

	setSize (1000, 930);
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::resized()
{
	guiControl.setBounds(0, 0, getWidth(),getHeight());

}


//MenuBarCallbacks==============================================================
StringArray MainContentComponent::getMenuBarNames()
{
	const char* const names[] = { "File", "Edit", 0 };
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
    PopupMenu editMenu;
	if (topLevelMenuIndex == 0)
	{
		fileMenu.addItem(OpenFile, "Open Files", true, false);
		fileMenu.addItem(OpenDirectory, "Open Directory", true, false);
		fileMenu.addSeparator();
        fileMenu.addItem(AudioPrefs, "Audio Preferences", true, false);
		fileMenu.addItem(ImportItunes, "Import Itunes Library", true, false);
        return fileMenu;
	}
    else
    {
        if (singletonUndoManager->canUndo())
        {
            String undoMessage("Undo ");
            undoMessage << singletonUndoManager->getUndoDescription();
            editMenu.addItem(UndoChange, undoMessage);
        }
        else
        {
            editMenu.addItem(UndoChange, "Undo", false);
        }

        
        return editMenu;
    }
	
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
					filesFound<<fileFound.getParentDirectory().getFileName()<<"/"<<fileFound.getFileName()<<"\n";
                    DBG("Loading : " << fileFound.getFileName());
                    singletonLibraryTree.addChild(TagReader::addToLibrary(fileFound),-1,0);
				}
				filesInfo << "You picked: " << initialDirectory << "\n" << "Files: \n" << filesFound;   
				AlertWindow::showMessageBoxAsync (AlertWindow::InfoIcon, "Directory Chooser...", filesInfo);
			}		
		}
		
		if (menuItemID == ImportItunes)
		{
			if(AlertWindow::showOkCancelBox (AlertWindow::WarningIcon,
											 "Itunes Import",
											 "Importing Itunes library will merge current Library",
											 "Merge Itunes Library",
											 "Keep Current Library",
											 0))
			{
				ITunesLibrary::getInstance()->setLibraryFile (ITunesLibrary::getDefaultITunesLibraryFile());
			}
		}
		
    }
    if (topLevelMenuIndex == EditMenu)
    {
        if (menuItemID == UndoChange)
        {
            if (singletonUndoManager->undoCurrentTransactionOnly())
            {
                tableUpdateRequired.setValue(true);
            }
            else
            {
                DBG("UNDO WENT WRONG");
            }
        }
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