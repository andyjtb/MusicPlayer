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
#include "MusicLibraryHelpers.h"

class TagReader
{	
public:
	static void print()
	{std::cout << "namespace\n";}	
	
	static ValueTree readTags (File& audioFile)
	{
		
		if (audioFile.getFullPathName().endsWith(".mp3")) {
			ValueTree tags("Tags");
			TagLib::MPEG::File f(audioFile.getFullPathName().toUTF8(), true, TagLib::AudioProperties::Average);	
			
			//		Dummy,
			//		LibID,
			//		ID,
			tags.setProperty(MusicColumns::columnNames[MusicColumns::Artist], f.tag()->artist().toCString(), nullptr);
			tags.setProperty(MusicColumns::columnNames[MusicColumns::Song], f.tag()->title().toCString(), nullptr);
			tags.setProperty(MusicColumns::columnNames[MusicColumns::Album], f.tag()->album().toCString(), nullptr);
			//		Rating,
			//		BPM,
			tags.setProperty(MusicColumns::columnNames[MusicColumns::Genre], f.tag()->genre().toCString(), nullptr);
			//		SubGenre,
			//		Label,
			//		Key,
			
			//DOESNT WORK
			tags.setProperty(MusicColumns::columnNames[MusicColumns::Length], timeHelpers::secondsToTimeLength(static_cast <double> (f.audioProperties()->length())), nullptr);
			DBG(timeHelpers::secondsToTimeLength(static_cast <double> (f.audioProperties()->length())));
			
			
			//		Kind,
			//		Added,
			//        Modified,
			//		Location,
			//		Score,
			
			return tags;
			
		}
		else {
			return ValueTree();
		}

	}
	
	static Image getAlbumArt (File& audioFile)
	{
		if (audioFile.getFullPathName().endsWith(".mp3")) {
			
			TagLib::MPEG::File f(audioFile.getFullPathName().toUTF8(), false, TagLib::AudioProperties::Average);
			
			TagLib::ID3v2::FrameList frames = f.ID3v2Tag()->frameList("APIC");
			
			if(frames.isEmpty())
			{
				return Image();
			}
			
			TagLib::ID3v2::AttachedPictureFrame *frame = static_cast<TagLib::ID3v2::AttachedPictureFrame *>(frames.front());
			
			TagLib::ByteVector imageData = frame->picture();
			
			Image juceCover = ImageFileFormat::loadFrom(imageData.data(), imageData.size());
			
			//return Image(frame->mimeType(), frame->picture());
			return juceCover;
		}
		else
		  {
			return Image();
		}
	}
};
#endif //H_TAGREADER