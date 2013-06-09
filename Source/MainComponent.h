/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT
#define MAINCOMPONENT

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioControl.h"
#include "GuiControl.h"
#include "Settings.h"
//#include "XmlHelpers.h"
#include "TagReader.h"
#include "DirectoryLoader.h"

#include "RemoteControl.h"


//==============================================================================
/**
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component,
							   public MenuBarModel,
                               public ApplicationCommandTarget
{
public:
    //==============================================================================
    /** Constructor. */
    MainContentComponent();
    /** Destructor. */
    ~MainContentComponent();
	
    /** @internal
     */
    void resized();
    
	//MenuBarEnums/Callbacks========================================================
    /**
	 Provides a named way of using the MenuBar options
	 */
    enum Menus
	{
		FileMenu=0,
        EditMenu=1,
        ControlMenu,
		
		NumMenus
	};
    
    /**
	 Provides a named way of using the MenuBar options
	 */
    enum FileMenuItems 
	{
		AddFile = 1,
		AddDirectory = 2,
		AudioPrefs = 3,
		ImportItunes = 4,
        UndoChange = 5,
        EffectsMenu = 6,
        LastFmMenu = 7,
		
		NumFileItems
	};
    
    /**
     Similar to the MenuBar enum, except it is used to perform actions based on keyboard shortcuts, uses unicode hex values to not conflict with the MenuBar int values
     */
    enum CommandIDs
    {
        undoMenu = 0x2000,
        
        spaceBar = 0x3000,
        nextTrack = 0x3100,
        previousTrack = 0x3200,
        volumeUp = 0x3300,
        volumeDown = 0x3400,
        
        search = 0x4000
    };
    
    /**
	 Used to create the top MenuBar
	 @return An array of the MenuBar titles
	 */
    StringArray getMenuBarNames();
    /**
     Creates a popup menu when a MenuBar title is chosen
	 @return Popup menu for that MenuBar option
	 */
    PopupMenu getMenuForIndex (int topLevelMenuIndex, const String& menuName);
    /**
	 Called when a menuItem is selected.
	 @param menuItemID The number of the menuItem selected
	 @param topLevelMenuIndex The number of the MenuBar item chosen
	 */
	void menuItemSelected (int menuItemID, int topLevelMenuIndex); 

    /** 
     Returns the next parent component that is an ApplicationCommandTarget
     */
    ApplicationCommandTarget* getNextCommandTarget();
    /** 
     Fills an array of all commands that can be performed by this target
     */
    void getAllCommands (Array <CommandID>& commands);
    /** 
     Provides information about the command that can be performed, used to display the action and key command in the top menu bar
     */
    void getCommandInfo (CommandID commandID, ApplicationCommandInfo& result);
    /** 
     Called when a key command is recieved, performs the related action
     */
    bool perform (const InvocationInfo& info);
    
private:
	AudioControl audioControl;
	GuiControl guiControl;

    RemoteControl remoteControl;

    ApplicationCommandManager commandManager;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  //MAINCOMPONENT
