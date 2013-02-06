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
                
                singletonCurrentValueTreeId = singletonCurrentValueTreeId+2;
                
                tags.setProperty(MusicColumns::columnNames[MusicColumns::ID],singletonCurrentValueTreeId, nullptr);
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
	
	static ImageWithType getAlbumArt (File& audioFile)
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
                        
                        ImageWithType juceCover;
                        juceCover.image = ImageFileFormat::loadFrom(imageData.data(), imageData.size());
                        juceCover.type = String(frame->mimeType().toCString()).fromFirstOccurrenceOf("/", false, true);
                        
                        return juceCover;
                    }
                }
            }
        }
        return ImageWithType();
	}

    static void saveAlbumArt (File& audioFile, Image& newCover, String imageType)
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
                            TagLib::ByteVector byteVector;
                            
                            readJuceImageToByteVector(newCover, imageType, byteVector);
                            
                            if (imageType == "JPEG")
                                frame->setMimeType("image/jpeg");
                            else
                                frame->setMimeType("image/png");
                            
                            frame->setPicture(byteVector);
                            
                            f.save();
                            
                        }
                    }
                    else
                    {
                        TagLib::ByteVector byteVector;
                        
                        readJuceImageToByteVector(newCover, imageType, byteVector);
                        
                        TagLib::ID3v2::AttachedPictureFrame* frame = new TagLib::ID3v2::AttachedPictureFrame;
                        f.ID3v2Tag()->addFrame(frame);
                        
                        //frame->setType(TagLib::ID3v2::AttachedPictureFrame::FrontCover);
                        
                        if (imageType == "JPEG")
                            frame->setMimeType("image/jpeg");
                        else
                            frame->setMimeType("image/png");
                        
                        frame->setPicture(byteVector);
                        
                        
                        f.save();
                    }
                    
                }
                  
            }
        }
    }
    
    static void readJuceImageToByteVector (Image& newCover, String imageType, TagLib::ByteVector& fillVector)
    {
        MemoryOutputStream newCoverData;
        
        if (imageType == "JPEG")
        {
            JPEGImageFormat image;
            image.writeImageToStream(newCover, newCoverData);
        }
        else
        {
            PNGImageFormat image;
            image.writeImageToStream(newCover, newCoverData);
        }
        
        fillVector.setData(static_cast<const char*>(newCoverData.getData()), newCoverData.getDataSize());                           
    }
    
    static String fileImageToMemoryBlock (File& audioFile, MemoryBlock& fillBlock)
    {
        //Returns image type
        String type = "none";
        ImageWithType currentCover = getAlbumArt(audioFile);
        
        //Makes sure memory block is sent to trigger receiving art bool to change
        fillBlock.setSize(10, true);
        
        if (currentCover.image.isValid())
        {
            TagLib::MPEG::File f(audioFile.getFullPathName().toUTF8(), false, TagLib::AudioProperties::Average);
            TagLib::ID3v2::FrameList frames = f.ID3v2Tag()->frameList("APIC");
            TagLib::ID3v2::AttachedPictureFrame *frame = static_cast<TagLib::ID3v2::AttachedPictureFrame *>(frames.front());
            
            if (frame != nullptr)
            {
                MemoryOutputStream outputStream;
                type = currentCover.type;
                
                if (type == "image/jpeg")
                {
                    JPEGImageFormat image;
                    image.writeImageToStream(currentCover.image, outputStream);
                    type = "jpeg";
                }
                else
                {
                    JPEGImageFormat image;
                    image.writeImageToStream(currentCover.image, outputStream);
                    type = "png";
                }

                fillBlock.replaceWith(outputStream.getData(), outputStream.getDataSize());
                
                return type;
            }
        }
        return type;
    }
    
    static bool compareAlbumArt (File& firstTrack, File& secondTrack)
    {
        MemoryBlock firstBlock, secondBlock;
        
        fileImageToMemoryBlock(firstTrack, firstBlock);
        fileImageToMemoryBlock(secondTrack, secondBlock);
        
        if (firstBlock == secondBlock)
        {
            return true;
        }
        else
            return false;
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