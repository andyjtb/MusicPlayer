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

			TagLib::ID3v2::FrameList bpmFrame = f.ID3v2Tag()->frameList("TBPM");
			if (!bpmFrame.isEmpty()) {
				tags.setProperty(MusicColumns::columnNames[MusicColumns::BPM], 0, nullptr);
			}
			
			tags.setProperty(MusicColumns::columnNames[MusicColumns::Genre], f.tag()->genre().toCString(), nullptr);
			//		SubGenre,
			//		Label,
			//		Key,
			
			//DOESNT WORK
			TagLib::ID3v2::FrameList lengthFrame = f.ID3v2Tag()->frameList("TLEN");
			
			if (!lengthFrame.isEmpty()) {
				TagLib::ID3v2::TextIdentificationFrame *frame = static_cast<TagLib::ID3v2::TextIdentificationFrame *>(lengthFrame.front()); 
				DBG("Length = " << frame->toString().toCString());
				
				tags.setProperty(MusicColumns::columnNames[MusicColumns::Length], frame->toString().toCString(), nullptr);
			}	
			else {
				tags.setProperty(MusicColumns::columnNames[MusicColumns::Length], timeHelpers::secondsToTimeLength(static_cast <double> (f.audioProperties()->length())), nullptr);
			}

			//		Kind,
			tags.setProperty(MusicColumns::columnNames[MusicColumns::Added], Time::getCurrentTime().toMilliseconds(), nullptr);
			tags.setProperty(MusicColumns::columnNames[MusicColumns::Modified], Time::getCurrentTime().toMilliseconds(), nullptr);
			tags.setProperty(MusicColumns::columnNames[MusicColumns::Location], audioFile.getFullPathName(), nullptr);
			//		Score,
			
			
			ScopedPointer<XmlElement> treeAsXml (tags.createXml());
			File testFile("~/tagTest.xml");
			treeAsXml->writeToFile (testFile, String::empty, "UTF-8", 200);			
			
			
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

			return juceCover;
		}
		else
		  {
			return Image();
		}
	}
};
#endif //H_TAGREADER