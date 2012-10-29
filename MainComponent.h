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
#include "MusicLibraryTable.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component,
							   public MenuBarModel
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
		
		NumMenus
	};
    
    enum FileMenuItems 
	{
		OpenFile = 1,
		OpenDirectory = 2,
		AudioPrefs = 3,
		testSave = 4,
		
		NumFileItems
	};
    StringArray getMenuBarNames();
    PopupMenu getMenuForIndex (int topLevelMenuIndex, const String& menuName);
	void menuItemSelected (int menuItemID, int topLevelMenuIndex); 
	
private:
	
	AudioControl audioControl;
	GuiControl guiControl;

	File testFile;
	
	//MusicLibraryTable musicTable;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  //MAINCOMPONENT
