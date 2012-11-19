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
#include "Settings.h"
#include "CoverFlowComponent.h"
#include "XmlHelpers.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component,
							   public MenuBarModel,
							   public ActionListener
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
		ImportItunes = 4,
		
		NumFileItems
	};
    StringArray getMenuBarNames();
    PopupMenu getMenuForIndex (int topLevelMenuIndex, const String& menuName);
	void menuItemSelected (int menuItemID, int topLevelMenuIndex); 
	
	void actionListenerCallback(const String& message);
//	static void alertBoxResultChosen (int result);
    void checkFirstTime();
	
private:
	
	AudioControl audioControl;
	GuiControl guiControl;

	File testFile;
	
	MusicLibraryTable musicTable;
    
//    CoverFlowComponent coverFlowComponent;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  //MAINCOMPONENT
