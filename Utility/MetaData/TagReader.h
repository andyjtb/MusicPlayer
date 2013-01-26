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
#include "Settings.h"

#include <cstdio>

class TagReader
{	
public:
	static ValueTree addToLibrary (File& audioFile)
	{
        //Only things required in the if are the declaration of f and the setProperty kind. Sort that out bruv
        //Also remember to add a thread called DuplicateRemover or similar which searches the library for duplicates in the background and removes them
		if (audioFile.existsAsFile())
        {
			if (audioFile.getFullPathName().endsWith(".mp3")) 
            {
                ValueTree tags("ITEM");
                TagLib::MPEG::File f(audioFile.getFullPathName().toUTF8());	
                
//                if (f != NULL)
//                {
                
                tags.setProperty(MusicColumns::columnNames[MusicColumns::ID], ++singletonCurrentValueTreeId, nullptr);
                tags.setProperty(MusicColumns::columnNames[MusicColumns::LibID], ++singletonCurrentLibId, nullptr);
                tags.setProperty(MusicColumns::columnNames[MusicColumns::Artist], f.tag()->artist().toCString(), nullptr);
                tags.setProperty(MusicColumns::columnNames[MusicColumns::Song], f.tag()->title().toCString(), nullptr);
                tags.setProperty(MusicColumns::columnNames[MusicColumns::Album], f.tag()->album().toCString(), nullptr);
                //		Rating,
                
//                TagLib::ID3v2::FrameList bpmFrame = f.ID3v2Tag()->frameList("TBPM");
//                if (!bpmFrame.isEmpty()) {
//                    tags.setProperty(MusicColumns::columnNames[MusicColumns::BPM], 0, nullptr);
//                }
                
                tags.setProperty(MusicColumns::columnNames[MusicColumns::Genre], f.tag()->genre().toCString(), nullptr);         
                tags.setProperty(MusicColumns::columnNames[MusicColumns::Label], String (CharPointer_UTF8(f.tag()->comment().toCString())), nullptr);
                
                AudioFormatManager formatManager;
                formatManager.registerBasicFormats();
                
                ScopedPointer<AudioFormatReader> reader;
                reader = formatManager.createReaderFor (audioFile);
                
                //Problem with 65days one time for all time album, fine for other mp3s
                if(reader != nullptr)
                {
                    int32 length = ((reader->lengthInSamples/reader->sampleRate)*1000);            
                    tags.setProperty(MusicColumns::columnNames[MusicColumns::Length],length , nullptr);
                }
                tags.setProperty(MusicColumns::columnNames[MusicColumns::Kind], "MPEG audio file", nullptr);
                tags.setProperty(MusicColumns::columnNames[MusicColumns::Added], Time::getCurrentTime().toMilliseconds(), nullptr);
                tags.setProperty(MusicColumns::columnNames[MusicColumns::Modified], audioFile.getLastModificationTime().toMilliseconds(), nullptr);
                tags.setProperty(MusicColumns::columnNames[MusicColumns::Location], audioFile.getFullPathName(), nullptr);
                
                tags.setProperty(MusicColumns::columnNames[MusicColumns::TrackNum], int(f.tag()->track()), nullptr);
                
//                ScopedPointer<XmlElement> treeAsXml (tags.createXml());
//                File testFile("~/tagTest.xml");
//                treeAsXml->writeToFile (testFile, String::empty, "UTF-8", 200);			
                
                
                return tags;
//            }
//                DBG("Tags not present");
//                return ValueTree();
            }
            DBG("FILE DOESNT EXIST");
            return ValueTree();
		}
		else {
			return ValueTree();
		}

	}
	
	static Image getAlbumArt (File& audioFile)
	{
        if (audioFile.exists())
        {
            if (audioFile.getFullPathName().endsWith(".mp3")) {
                
                TagLib::MPEG::File f(audioFile.getFullPathName().toUTF8(), false, TagLib::AudioProperties::Average);
                
                TagLib::ID3v2::FrameList frames = f.ID3v2Tag()->frameList("APIC");
                
                if(!frames.isEmpty())
                {
                    TagLib::ID3v2::AttachedPictureFrame *frame = static_cast<TagLib::ID3v2::AttachedPictureFrame *>(frames.front());
                    
                    if (frame != nullptr)
                    {
                        TagLib::ByteVector imageData = frame->picture();
                        
                        Image juceCover = ImageFileFormat::loadFrom(imageData.data(), imageData.size());
                        
                        return juceCover;
                    }
                }
                
            }
        }
        return Image();
	}

    static void saveAlbumArt (File& audioFile, Image& newCover)
    {
        if (audioFile.exists())
        {
            if (audioFile.getFullPathName().endsWith(".mp3")) {
                
                TagLib::MPEG::File f(audioFile.getFullPathName().toUTF8(), false, TagLib::AudioProperties::Average);

                if(f.ID3v2Tag())
                {
                    TagLib::ID3v2::FrameList frames = f.ID3v2Tag()->frameList("APIC");
                    
                    if(!frames.isEmpty())
                    {
                        TagLib::ID3v2::AttachedPictureFrame *frame = static_cast<TagLib::ID3v2::AttachedPictureFrame *>(frames.front());
                        
                        if (frame != nullptr)
                        {
//                            DBG("Save Album Art");
//                            FILE * fout;
//                            fout = fopen("/testPicture.jpg", "wb");
//                            fwrite(frame->picture().data(), frame->picture().size(), 1, fout);
//                            fclose(fout);
                            //Works, saves album art from tag to file
                            Image::BitmapData bm (newCover, Image::BitmapData::readOnly);
                            TagLib::ByteVector byteVector;
                            byteVector.setData(reinterpret_cast<char*>(bm.data));
                            
                            DBG("Size = " << String(byteVector.size()));
                            
                            FILE * fout;
                            fout = fopen("/testPicture.jpg", "wb+");
                            fwrite(byteVector.data(), byteVector.size(), 1, fout);
                            fclose(fout);
                            
                        }
                    }
                }
                    else
                    {
                        DBG("No APIC");
                    }
            }
        }
    }
    
    static int getBitRate (File& audioFile)
    {
        TagLib::MPEG::File f(audioFile.getFullPathName().toUTF8());
        return f.audioProperties()->bitrate();
    }
    
    static void writeTag (int columnId, ValueTree incomingTrack)
    {
        TagLib::MPEG::File f(incomingTrack.getProperty(MusicColumns::columnNames[MusicColumns::Location]).toString().toUTF8());	
        
        switch (columnId) {
            case 3:
                //Artist
                f.tag()->setArtist(incomingTrack.getProperty(MusicColumns::columnNames[MusicColumns::Artist]).toString().toWideCharPointer());
                break;
            case 4:
                //Song
                f.tag()->setTitle(incomingTrack.getProperty(MusicColumns::columnNames[MusicColumns::Song]).toString().toWideCharPointer());
                break;
            case 5:
                //Album
                f.tag()->setAlbum(incomingTrack.getProperty(MusicColumns::columnNames[MusicColumns::Album]).toString().toWideCharPointer());
                break;
            case 8:
                //Genre
                f.tag()->setGenre(incomingTrack.getProperty(MusicColumns::columnNames[MusicColumns::Genre]).toString().toWideCharPointer());
                break;
            case 10:
                //Label
                f.tag()->setComment(incomingTrack.getProperty(MusicColumns::columnNames[MusicColumns::Label]).toString().toWideCharPointer());
                break;
            case 18:
                //TrackNum
                f.tag()->setTrack(int(incomingTrack.getProperty(MusicColumns::columnNames[MusicColumns::TrackNum])));
                break;
            default:
                break;
        }
        f.save();
        
    }
    
 };
#endif //H_TAGREADER