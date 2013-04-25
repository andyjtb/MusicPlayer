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

struct EqDetails{
    int ID;
    Value On;
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
#define singletonEqSettings Settings::getInstance()->getEqSettings()

#define tableSelectedTracks Settings::getInstance()->getSelectedTracks()
#define tablePlayingRow Settings::getInstance()->getPlayingRow()
#define tableUpdateRequired Settings::getInstance()->getUpdateRequired()
#define tableLayoutString Settings::getInstance()->getTableLayout()

#define artClipboard Settings::getInstance()->getArtClipboard()
#define artUpdateRequired Settings::getInstance()->getAlbumArtUpdate()
#define filteredDataList Settings::getInstance()->getFilteredList()

#define remoteConnections Settings::getInstance()->getConnections()

#define currentEqDetails Settings::getInstance()->getCurrentEq()
/** A singleton class containing a number of variables used frequently throughout the application
 */
class Settings : public DeletedAtShutdown
{
public:
    /** Constructor. */
	Settings();
	/** Destructor. */
	~Settings();

    /** Checks to see if this is the first time the application is being run, if it is it creates all of the necessary files, Settings, libraryFile, playlistFile, eqFile.
     @return Whether or not this is the first time the application has been run
     */
    bool checkFirstTime();
    /** Creates a new Settings.xml file containing default library, playlist, table layout and eq information
     @return Whether or not it was successful
     */
    bool createSettingsFile();
    
    /** The location of the file where the library is saved
     */
	File& getLibraryFile();
    /** The location of the file where playlist information is saved
     */
    File& getPlaylistsFile();
	
    /** The global undo manager
     */
    UndoManager* getUndoManager();
    
    /** The lib id of the last track added to the library
     */
    int& getCurrentLibId();
    /** The track ID of the last track added to the library
     */
    int& getCurrentValueTreeId();
    /** The unique identifer given to the last added playlist
     */
    int& getCurrentPlaylistId();
    /** An array containing the track IDs of the items currently selected in the MusicLibraryTable, used when mulitple items are selected
     */
    Array<int>& getSelectedTracks();
    
    /** An array of the different remote control connection objects
     */
    OwnedArray <RemoteInterprocessConnection, CriticalSection>& getConnections();
    
    /** The ValueTree which stores the whole music library
     */
    ValueTree& getLibraryTree();
    /** The ValueTree which stores all of the playlists
     */
    ValueTree& getPlaylistsTree();
    /** The ValueTree representing the row that is currently selected in the MusicLibraryTable
     */
    ValueTree& getSelectedRow();
    /** The ValueTree representing the audio file that is currently being played
     */
    ValueTree& getPlayingRow();
    /** The ValueTree being displayed in the MusicLibraryTable
     */
    ValueTree& getFilteredList();
    /** A valuetree containing all eq presets
     */
    ValueTree& getEqSettings(){return eqSettings;}
    
    /** A value which tells the application to load a file and then begin playing it
     */
    Value& getShouldPlay();
    /** A value which shows whether the application is playing audio or not
     */
    Value& getPlayState();
    /** A value which triggers the music table to update
     */
    Value& getUpdateRequired();

    /** A value which is used to trigger all album art displaying components to update their display
     */
    Value& getAlbumArtUpdate();
    
    /** A variable whos value is used to denote whether the application is to load an audio file into the audio reader but no play it
     */
    Value& getLoadSelected(){return loadSelected;}
    
    /** A container which holds an ImageWithType that is being stored in the "clipboard" after being copied by the user
     */
    ImageWithType& getArtClipboard();
    
    /** A containter which holds details of which EQ preset is being used and whether or not it is on, used for saving on program exit.
     */
    EqDetails& getCurrentEq() {return eqDetails;}
    
    /** Returns the MusicLibraryTable layout as a String containing whether each column is visible, its width and position
  
     */
    String& getTableLayout() {return tableLayout;}
    
    /** Saves settings that are to be recalled: Library, Playlists, Current EQ, Table layout
     */
    void saveSingletons();
	
	juce_DeclareSingleton_SingleThreaded_Minimal (Settings)
	
private:
    /** @internal. */
    void initSettings();
    
    File settingsXmlFile, libraryFile, playlistFile, eqSettingsFile;
	ValueTree libraryTree, playlistTree, selectedRow, playingRow, filteredDataTree, eqSettings;
    ScopedPointer<UndoManager> undoManager;
    int currentLibId, currentValueTreeId, currentPlaylistId;
    EqDetails eqDetails;
    Array<int> selectedTracks;
    Value shouldPlay, loadSelected, playState, updateRequired, artUpdate;
    OwnedArray <RemoteInterprocessConnection, CriticalSection> connections;
    ImageWithType albumArtClipboard;
    String tableLayout;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Settings);
};

#endif //H_SETTINGS