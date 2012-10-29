/*
 *  TagReader.h
 *  MusicPlayer
 *
 *  Created by Andy on 27/10/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef H_TAGREADER
#define H_TAGREADER

#include "../JuceLibraryCode/JuceHeader.h"
#include "TagLibAmalgam.h"

class TagReader
{	
public:
	static void print()
	{std::cout << "namespace\n";}	
	
	static ValueTree readTags (File& audioFile)
	{
		ValueTree tags("Tags");
		
		TagLib::FileRef f(audioFile.getFullPathName().toUTF8());
		String artist = f.tag()->artist().toCString();
		
		DBG(artist);
		
		return tags;
	}
};
#endif //H_TAGREADER