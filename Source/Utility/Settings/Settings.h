#ifndef H_SETTINGS
#define H_SETTINGS

#include "../JuceLibraryCode/JuceHeader.h"
#include "Utility.h"
#include "MusicLibraryHelpers.h"

//Forward Declaration
//#include "RemoteInterprocessConnection.h"
class RemoteInterprocessConnection;

struct ImageWithType {
    Image image;
    String type;
};

#define singletonLibraryFile Settings::getInstance()->getLibraryFile()
#define singletonLibraryTree Settings::getInstance()->getLibraryTree()
#define singletonPlaylistsFile Settings::getInstance()->getPlaylistsFile()
#define singletonPlaylistsTree Settings::getInstance()->getPlaylistsTree()
#define singletonCurrentLibId Settings::getInstance()->getCurrentLibId()
#define singletonCurrentValueTreeId Settings::getInstance()->getCurrentValueTreeId()
#define singletonCurrentPlaylistId Settings::getInstance()->getCurrentPlaylistId()
#define singletonPlayState Settings::getInstance()->getPlayState()
#define singletonUndoManager Settings::getInstance()->getUndoManager()

#define tableSelectedRow Settings::getInstance()->getSelectedRow()
#define tableSelectedTracks Settings::getInstance()->getSelectedTracks()
#define tableShouldPlay Settings::getInstance()->getShouldPlay()
#define tableLoadSelected Settings::getInstance()->getLoadSelected()
#define tablePlayingRow Settings::getInstance()->getPlayingRow()
#define tableUpdateRequired Settings::getInstance()->getUpdateRequired()
#define tableDeleting Settings::getInstance()->getTableDeleting()

#define artClipboard Settings::getInstance()->getArtClipboard()
#define artUpdateRequired Settings::getInstance()->getAlbumArtUpdate()
#define filteredDataList Settings::getInstance()->getFilteredList()

#define remoteConnections Settings::getInstance()->getConnections()

class Settings : public DeletedAtShutdown
{
public:
	Settings();
	
	~Settings();

	File& getLibraryFile();
    File& getPlaylistsFile();
	
    UndoManager* getUndoManager();
    
    int& getCurrentLibId();
    int& getCurrentValueTreeId();
    int& getCurrentPlaylistId();
    Array<int>& getSelectedTracks();
    
    OwnedArray <RemoteInterprocessConnection, CriticalSection>& getConnections();
    
    ValueTree& getLibraryTree();
    ValueTree& getPlaylistsTree();
    ValueTree& getSelectedRow();
    ValueTree& getPlayingRow();
    ValueTree& getFilteredList();
    
    Value& getShouldPlay();
    Value& getPlayState();
    Value& getUpdateRequired();
    Value& getTableDeleting();
    Value& getAlbumArtUpdate();
    Value& getLoadSelected();
    
    ImageWithType& getArtClipboard();
    
    void saveSingletons();
	
	juce_DeclareSingleton_SingleThreaded_Minimal (Settings)
	
private:

    File settingsXmlFile, libraryFile, playlistFile;
	ValueTree libraryTree, playlistTree, selectedRow, playingRow, filteredDataTree;
    ScopedPointer<UndoManager> undoManager;
    int currentLibId, currentValueTreeId, currentPlaylistId;
    Array<int> selectedTracks;
    Value shouldPlay, loadSelected, playState, updateRequired, deletingTable, artUpdate;
    OwnedArray <RemoteInterprocessConnection, CriticalSection> connections;
    ImageWithType albumArtClipboard;

};

#endif //H_SETTINGS