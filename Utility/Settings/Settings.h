#ifndef H_SETTINGS
#define H_SETTINGS

#include "../JuceLibraryCode/JuceHeader.h"
#include "Utility.h"
#include "MusicLibraryHelpers.h"


#define singletonLibraryFile Settings::getInstance()->getLibraryFile()
#define singletonLibraryTree Settings::getInstance()->getLibraryTree()
#define singletonCurrentLibId Settings::getInstance()->getCurrentLibId()
#define singletonCurrentValueTreeId Settings::getInstance()->getCurrentValueTreeId()
#define singletonPlayState Settings::getInstance()->getPlayState()
#define singletonUndoManager Settings::getInstance()->getUndoManager()

#define tableSelectedRow Settings::getInstance()->getSelectedRow()
#define tableShouldPlay Settings::getInstance()->getShouldPlay()
#define tablePlayingRow Settings::getInstance()->getPlayingRow()
#define tableUpdateRequired Settings::getInstance()->getUpdateRequired()
#define tableDeleting Settings::getInstance()->getTableDeleting()

#define filteredDataList Settings::getInstance()->getFilteredList()

#define remoteNumConnections Settings::getInstance()->getNumConnections()


class Settings : public DeletedAtShutdown
{
public:
	Settings();
	
	~Settings();

	File& getLibraryFile();
	
    UndoManager* getUndoManager();
    
    int& getCurrentLibId();
    int& getCurrentValueTreeId();
    int& getNumConnections();
    
    ValueTree& getLibraryTree();
    ValueTree& getSelectedRow();
    ValueTree& getPlayingRow();
    ValueTree& getFilteredList();
    
    Value& getShouldPlay();
    Value& getPlayState();
    Value& getUpdateRequired();
    Value& getTableDeleting();
    
    void saveSingletons();
	
	juce_DeclareSingleton_SingleThreaded_Minimal (Settings)
	
private:

    File settingsXmlFile, libraryFile;
	ValueTree libraryTree, selectedRow, playingRow, filteredDataTree;
    ScopedPointer<UndoManager> undoManager;
    int currentLibId, currentValueTreeId, numConnections;
    Value shouldPlay, playState, updateRequired, deletingTable;

};

#endif //H_SETTINGS