//
//  DirectoryLoader.h
//  MusicPlayer
//
//  Created by Andy on 29/12/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef MusicPlayer_DirectoryLoader_h
#define MusicPlayer_DirectoryLoader_h

#include "../JuceLibraryCode/JuceHeader.h"
#include "Settings.h"
#include "TagReader.h"

class DirectoryLoader : public ThreadWithProgressWindow
{
public:
    DirectoryLoader() : ThreadWithProgressWindow ("Loading...", false, true)
    {
    }
    void run()
    {
        const MessageManagerLock mmLock;
        
        String filesFound;
        
        DirectoryIterator directoryIterator (directory, true, wildcards,2);
        
        //setProgress(-1.0);
        
        while (directoryIterator.next())
        {
            //            setProgress(-1.0);
            if (threadShouldExit())
                break;
            File fileFound (directoryIterator.getFile());
            //            String loadingString("Loading : " + fileFound.getFileName());
            //            setStatusMessage(loadingString);
            singletonLibraryTree.addChild(TagReader::addToLibrary(fileFound),-1,0);
            
        }
    }
    
    void setDirectory (File incomingDirectory)
    {
        directory = incomingDirectory;
    }
    
    void setWildcards (String _wildcards)
    {
        wildcards = _wildcards;
    }
    
    String wildcards;
    File directory;
};

#endif
