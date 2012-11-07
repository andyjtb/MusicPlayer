

#ifndef H_SETTINGS
#define H_SETTINGS

#include "../JuceLibraryCode/JuceHeader.h"
#include "Utility.h"


#define singletonLibraryFile Settings::getInstance()->getLibraryFile()
#define singletonLibraryTree Settings::getInstance()->getLibraryTree()


class Settings : public DeletedAtShutdown
{
public:
	Settings();
	
	~Settings();

	File& getLibraryFile();
	
	ValueTree& getLibraryTree();

	
	juce_DeclareSingleton_SingleThreaded_Minimal (Settings)
	
private:

	File libraryFile;
	ValueTree libraryTree;
};

#endif //H_SETTINGS