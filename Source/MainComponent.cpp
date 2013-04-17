/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainContentComponent::MainContentComponent()
{
    remoteControl.setControls(&guiControl, &audioControl);
    
    commandManager.registerAllCommandsForTarget (this);
    commandManager.registerAllCommandsForTarget (JUCEApplication::getInstance());
    
    // this lets the command manager use keypresses that arrive in our window to send
    // out commands
    addKeyListener (commandManager.getKeyMappings());
    setApplicationCommandManagerToWatch (&commandManager);
    
    addAndMakeVisible(&guiControl);
	guiControl.setAudioControl(&audioControl);
    
	setSize (1000, 630);
    
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
	const char* const names[] = {"File", "Edit", "Controls", 0 };
	return StringArray (names);
}

PopupMenu MainContentComponent::getMenuForIndex (int topLevelMenuIndex, const String& menuName)
{
	PopupMenu menu;
    
	if (topLevelMenuIndex == 0)
	{
		menu.addItem(AddFile, "Add Files", true, false);
		menu.addItem(AddDirectory, "Add Directory", true, false);
		menu.addSeparator();
        /*
        menu.addItem(AudioPrefs, "Audio Preferences", true, false);
        */
		menu.addItem(ImportItunes, "Import Itunes Library", true, false);
        menu.addSeparator();
        menu.addCommandItem (&commandManager, StandardApplicationCommandIDs::quit);
        return menu;
	}
    else if (topLevelMenuIndex == 1)
    {
        menu.addCommandItem (&commandManager, undoMenu);
        menu.addSeparator();
        menu.addItem(EffectsMenu, "Effects");
        return menu;
    }
    else
    {
        menu.addCommandItem(&commandManager, spaceBar);
        return menu;
    }
}

void MainContentComponent::menuItemSelected (int menuItemID, int topLevelMenuIndex)
{
    if (topLevelMenuIndex == -1)
    {
        //App name menu
        if (menuItemID == AudioPrefs) 
        {
            audioControl.showAudioPreferences(this);
        }
    }
    
	if (topLevelMenuIndex == FileMenu) 
    {
        
        if (menuItemID == AddFile)
		{
			FileChooser fc ("Choose files to add...",File::getCurrentWorkingDirectory(),"*",true);
			
			if (fc.browseForMultipleFilesToOpen())
			{
                Array<File> selectedFiles = fc.getResults();
                File audioFile;
                
				for (int i = 0; i < selectedFiles.size(); ++i)
				{
                    audioFile = selectedFiles[i];
                    singletonLibraryTree.addChild(TagReader::addToLibrary(audioFile),-1,0);
				}
                
//				audioControl.loadFile(audioFile);
//				guiControl.updateTagDisplay(audioFile);
			}
		}
		
		if (menuItemID == AddDirectory)
		{
			FileChooser fc ("Choose a directory to add...",File::getCurrentWorkingDirectory(),"*",true);
			if (fc.browseForDirectory()) 
			{
                DirectoryLoader d;
                d.setDirectory(fc.getResult());
                
                if (d.runThread())
                {
                    DBG("Complete");
                }
                else
                {
                    DBG("User quit early");
                }
            }
		}
		
		if (menuItemID == ImportItunes)
		{
//			if(AlertWindow::showOkCancelBox (AlertWindow::WarningIcon,
//											 "Itunes Import",
//											 "Importing Itunes library will merge current Library",
//											 "Merge Itunes Library",
//											 "Keep Current Library",
//											 0))
			if(NativeMessageBox::showOkCancelBox (AlertWindow::WarningIcon,
                                                  "iTunes Import",
                                                  "Importing iTunes library will merge current Library"
                                                  ))

			{
                if(ITunesLibrary::getDefaultITunesLibraryFile().exists())
                {
                    ITunesLibrary::getInstance()->setLibraryFile (ITunesLibrary::getDefaultITunesLibraryFile());
                }
                else
                {
                    DBG("Library file not found/selected");
                }
			}
		}
		
    }
    if (topLevelMenuIndex == EditMenu)
    {
        if (menuItemID == EffectsMenu)
        {
            guiControl.showEffectsMenu();
        }
    }
    
    if (topLevelMenuIndex == ControlMenu)
    {
        
    }
}

ApplicationCommandTarget* MainContentComponent::getNextCommandTarget()
{
    // this will return the next parent component that is an ApplicationCommandTarget (in this
    // case, there probably isn't one, but it's best to use this method in your own apps).
    return findFirstTargetParentComponent();
}

void MainContentComponent::getAllCommands (Array <CommandID>& commands)
{
    // this returns the set of all commands that this target can perform..
    const CommandID ids[] = {undoMenu, spaceBar};
    
    commands.addArray (ids, numElementsInArray (ids));
}

// This method is used when something needs to find out the details about one of the commands
// that this object can perform..
void MainContentComponent::getCommandInfo (CommandID commandID, ApplicationCommandInfo& result)
{
    const String generalCategory ("General");
    
    switch (commandID)
    {
        case undoMenu:
            if (singletonUndoManager->canUndo()) {
                String undoMessage("Undo ");
                undoMessage << singletonUndoManager->getUndoDescription();
                
                result.setInfo (undoMessage, "Undo Last Action", generalCategory, 0);
                result.addDefaultKeypress ('z', ModifierKeys::commandModifier);
                break;
            }
            else
            {
                result.setInfo ("Undo", "Undo Last Action", generalCategory, 0);
                result.setActive(false);
                result.addDefaultKeypress ('z', ModifierKeys::commandModifier);
                break;
            }
            
        case spaceBar:
        {
            String playPause;
            singletonPlayState.getValue() ? playPause = "Pause" : playPause = "Play";
            result.setInfo(playPause, "Play or Pause", generalCategory, 0);
            result.addDefaultKeypress(KeyPress::spaceKey, 0);
            
        }                
        default:
            break;
    };
}

// this is the ApplicationCommandTarget method that is used to actually perform one of our commands..
bool MainContentComponent::perform (const InvocationInfo& info)
{
    switch (info.commandID)
    {
        case undoMenu:
            if (singletonUndoManager->undoCurrentTransactionOnly())
            {
                tableUpdateRequired.setValue(true);
            }
            break;
        case spaceBar:
            singletonPlayState.getValue() ? singletonPlayState = false : singletonPlayState = true;
            break;
        default:
            return false;
    };
    
    return true;
}