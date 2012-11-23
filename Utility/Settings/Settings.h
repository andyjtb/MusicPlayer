#ifndef H_SETTINGS
#define H_SETTINGS

#include "../JuceLibraryCode/JuceHeader.h"
#include "Utility.h"
#include "MusicLibraryHelpers.h"


#define singletonLibraryFile Settings::getInstance()->getLibraryFile()
#define singletonLibraryTree Settings::getInstance()->getLibraryTree()
#define singletonCurrentLibId Settings::getInstance()->getCurrentLibId()
#define singletonCurrentValueTreeId Settings::getInstance()->getCurrentValueTreeId()
#define tableSelectedRow Settings::getInstance()->getSelectedRow()
#define tableShouldPlay Settings::getInstance()->getShouldPlay()


class Settings : public DeletedAtShutdown
{
public:
	Settings();
	
	~Settings();

	File& getLibraryFile();
	
	ValueTree& getLibraryTree();
    
    int& getCurrentLibId();
    int& getCurrentValueTreeId();
    Value& getSelectedRow();
    Value& getShouldPlay();

    void saveSingletons();
	
	juce_DeclareSingleton_SingleThreaded_Minimal (Settings)
	
private:

    File settingsXmlFile;
    
	File libraryFile;
	ValueTree libraryTree;
    int currentLibId, currentValueTreeId;
    Value selectedRow, shouldPlay;

};

#endif //H_SETTINGS