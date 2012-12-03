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
#include "XmlHelpers.h"
#include "TagReader.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component,
							   public MenuBarModel,
                               public ApplicationCommandTarget
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();
	
    void resized();

	//MenuBarEnums/Callbacks========================================================
    enum Menus
	{
		FileMenu=0,
        EditMenu=1,
        ControlMenu,
		
		NumMenus
	};
    
    enum FileMenuItems 
	{
		OpenFile = 1,
		OpenDirectory = 2,
		AudioPrefs = 3,
		ImportItunes = 4,
        UndoChange = 5,
		
		NumFileItems
	};
    
    enum CommandIDs
    {
        undoMenu = 0x2000,
        spaceBar = 0x3000
    };
    
    StringArray getMenuBarNames();
    PopupMenu getMenuForIndex (int topLevelMenuIndex, const String& menuName);
	void menuItemSelected (int menuItemID, int topLevelMenuIndex); 

    void checkFirstTime();
    
    ApplicationCommandTarget* getNextCommandTarget();
    void getAllCommands (Array <CommandID>& commands);
    void getCommandInfo (CommandID commandID, ApplicationCommandInfo& result);
    bool perform (const InvocationInfo& info);
    
private:
	AudioControl audioControl;
	GuiControl guiControl;

	File testFile;

    ApplicationCommandManager commandManager;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  //MAINCOMPONENT
