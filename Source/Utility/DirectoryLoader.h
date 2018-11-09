//
//  DirectoryLoader.h
//  MusicPlayer
//
//  Created by Andy on 29/12/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef MusicPlayer_DirectoryLoader_h
#define MusicPlayer_DirectoryLoader_h

#include "JuceHeader.h"
#include "Settings/Settings.h"
#include "MetaData/TagReader.h"
#include "Information/Info Bar/InfoBar.h"
/** A thread with a progress window used to recursively search through a directory for all files which match the wild card, adding them to the library
    @see TagReader
 */

class DirectoryLoader : public ThreadWithProgressWindow
{
public:
    DirectoryLoader() : ThreadWithProgressWindow ("Loading...", true, true)
    {
    }
    /**@internal
     */
    void run()
    {
        const MessageManagerLock mmLock;
        
        String filesFound;
        
        DirectoryIterator directoryIterator (directory, true, wildcards,2);
        
        //setProgress(-1.0);
        
        while (directoryIterator.next())
        {
            setProgress(-1.0);
            
            if (threadShouldExit())
                break;
            
            File fileFound (directoryIterator.getFile());
            
            //if (infoBar != nullptr)
            //    infoBar->loadingFile(fileFound);
            
            singletonLibraryTree.addChild(TagReader::addToLibrary(fileFound),-1,0);
            
        }
    }
    /** Sets the directory which is to be searched for appropriate files
     @param incomingDirectory The directory to be searched
     */
    void setDirectory (File incomingDirectory)
    {
        directory = incomingDirectory;
    }
    
//    void setInfoBar (InfoBar* _infoBar)
//    {
//        infoBar.set(_infoBar, false);
//    }
    /** Sets the file extensions which will decide whether a file is appropriate or not
     @param _wildcards The file extensions that the loader will load in the format *.fileExtension;*.otherFileExtension
     */
    void setWildcards (String _wildcards)
    {
        wildcards = _wildcards;
    }
    
private:
    File directory;
    OptionalScopedPointer<InfoBar> infoBar;
    String wildcards;
};

#endif
