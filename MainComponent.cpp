/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent()
{
    setSize (500, 400);
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colour (0xffeeddff));

    g.setFont (Font (16.0f));
    g.setColour (Colours::black);
    g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);
}

void MainContentComponent::resized()
{

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
		fileMenu.addItem(OpenFile, "Open Files", true, false);
		fileMenu.addItem(OpenDirectory, "Open Directory", true, false);
		fileMenu.addSeparator();
        fileMenu.addItem(AudioPrefs, "Audio Preferences", true, false);

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
				audioControl.setPlayState(true);
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
				filesInfo << "You picked: " << initialDirectory << "\n" << "Files: " << filesFound;   
				AlertWindow::showMessageBoxAsync (AlertWindow::InfoIcon, "Directory Chooser...", filesInfo);
			}		
			
		}

    }
}