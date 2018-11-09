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
        menu.addItem(LastFmMenu, "Enabled Last.fm Service", true, currentLastFm.getEnabled().getValue());
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
        menu.addSeparator();
        menu.addCommandItem(&commandManager, nextTrack);
        menu.addCommandItem(&commandManager, previousTrack);
        menu.addCommandItem(&commandManager, volumeUp);
        menu.addCommandItem(&commandManager, volumeDown);
        menu.addSeparator();
        menu.addCommandItem(&commandManager, search);
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
			FileChooser fc ("Choose files to add...", lastOpenedFile,audioControl.getRegisteredFormatWildcard(),true);
			
			if (fc.browseForMultipleFilesToOpen())
			{
                Array<File> selectedFiles = fc.getResults();
                File audioFile;
                
				for (int i = 0; i < selectedFiles.size(); ++i)
				{
                    audioFile = selectedFiles[i];
                    singletonLibraryTree.addChild(TagReader::addToLibrary(audioFile),-1,0);
				}
                
                lastOpenedFile = audioFile;
			}
		}
		
		else if (menuItemID == AddDirectory)
		{
			FileChooser fc ("Choose a directory to add...", lastOpenedFile,"*",true);
			if (fc.browseForDirectory()) 
			{
                DirectoryLoader d;
                d.setWildcards(audioControl.getRegisteredFormatWildcard());
                d.setDirectory(fc.getResult());
                
                d.runThread();
            }
            
            lastOpenedFile = fc.getResult();
		}
		
		else if (menuItemID == ImportItunes)
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
		
        else if (menuItemID == LastFmMenu)
            currentLastFm.getEnabled().setValue(!currentLastFm.getEnabled().getValue());
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
    const CommandID ids[] = {undoMenu, spaceBar, volumeUp, volumeDown, nextTrack, previousTrack, search};
    
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
            break;
        } 
        case search:
        {
            result.setInfo("Search Library", "Search Library", generalCategory, 0);
            result.addDefaultKeypress('f', ModifierKeys::commandModifier);
            break;
        }
            
        case volumeUp:
        {
            result.setInfo("Volume Up", "Volume Up", generalCategory, 0);
            result.addDefaultKeypress(KeyPress::upKey, ModifierKeys::commandModifier);
            break;
        }
        case volumeDown:
        {
            result.setInfo("Volume Down", "Volume Down", generalCategory, 0);
            result.addDefaultKeypress(KeyPress::downKey, ModifierKeys::commandModifier);
            break;
        }
        case nextTrack:
        {
            result.setInfo("Next Track", "Next Track", generalCategory, 0);
            result.addDefaultKeypress(KeyPress::rightKey, ModifierKeys::commandModifier);
            break;
        }
        case previousTrack:
        {
            result.setInfo("Previous Track", "Previous Track", generalCategory, 0);
            result.addDefaultKeypress(KeyPress::leftKey, ModifierKeys::commandModifier);
            break;
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
        case search:
            guiControl.getSearchBox().grabKeyboardFocus();
            break;
        case volumeUp:
        {
            if (audioControl.getVolume() < 0.9)
                guiControl.setVolume(audioControl.getVolume()+0.1);
            else
                guiControl.setVolume(1);
            break;
        }
        case volumeDown:
        {
            if (audioControl.getVolume() > 0.1)
                guiControl.setVolume(audioControl.getVolume()-0.1);
            else
                guiControl.setVolume(0);
            break;
        }
        case nextTrack:
            guiControl.next();
            break;
        case previousTrack:
            guiControl.previous();
            break;
        default:
            return false;
    };
    
    return true;
}
