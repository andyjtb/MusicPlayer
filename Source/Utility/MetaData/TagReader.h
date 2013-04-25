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
/** A utility class containing a number of different static functions used for metadata tag manipulation.
 */
class TagReader
{	
public:
    /** Reads metadata tags to fill ValueTree information in. Returns ValueTree with all of the relevant information about the file in the same format at the audio library 
     @param audioFile The file to be added to the library
     @return ValueTree containing all of the information about the file ready to add to the library
     */
	static ValueTree addToLibrary (const File& audioFile)
	{
		if (audioFile.existsAsFile())
        {
            AudioFormatManager formatManager;
            formatManager.registerBasicFormats();
            
            ScopedPointer<AudioFormatReader> reader;
            reader = formatManager.createReaderFor (audioFile);
            
            
			if (reader != nullptr) 
            {
                ValueTree tags("ITEM");
                TagLib::FileRef f(audioFile.getFullPathName().toUTF8());	

                singletonCurrentValueTreeId = singletonCurrentValueTreeId+2;
                
                tags.setProperty(MusicColumns::columnNames[MusicColumns::ID],singletonCurrentValueTreeId, nullptr);
                tags.setProperty(MusicColumns::columnNames[MusicColumns::LibID], ++singletonCurrentLibId, nullptr);
                tags.setProperty(MusicColumns::columnNames[MusicColumns::Artist], f.tag()->artist().toCString(), nullptr);
                
                //If no tag song title exists, set the song title to file name
                if (f.tag()->title().isEmpty())
                    tags.setProperty(MusicColumns::columnNames[MusicColumns::Song], audioFile.getFileName(), nullptr);
                else
                    tags.setProperty(MusicColumns::columnNames[MusicColumns::Song], f.tag()->title().toCString(), nullptr);
                    
                tags.setProperty(MusicColumns::columnNames[MusicColumns::Album], f.tag()->album().toCString(), nullptr);
                
                tags.setProperty(MusicColumns::columnNames[MusicColumns::Genre], f.tag()->genre().toCString(), nullptr);         
                tags.setProperty(MusicColumns::columnNames[MusicColumns::Label], String (CharPointer_UTF8(f.tag()->comment().toCString())), nullptr);
                
                tags.setProperty(MusicColumns::columnNames[MusicColumns::Size], audioFile.getSize(), nullptr);
                
                tags.setProperty(MusicColumns::columnNames[MusicColumns::BitRate], f.audioProperties()->bitrate(), nullptr);                                         
                
                //Problem with 65days one time for all time album, fine for other mp3s
                //if(reader != nullptr)
                //{
                    int32 length = ((reader->lengthInSamples/reader->sampleRate)*1000);            
                    tags.setProperty(MusicColumns::columnNames[MusicColumns::Length],length , nullptr);
                //}
                
                tags.setProperty(MusicColumns::columnNames[MusicColumns::SampleRate], reader->sampleRate, nullptr);
                
                if (audioFile.getFileExtension() == "mp3")
                    tags.setProperty(MusicColumns::columnNames[MusicColumns::Kind], "MPEG audio file", nullptr);
                else
                    tags.setProperty(MusicColumns::columnNames[MusicColumns::Kind], reader->getFormatName(), nullptr);
                
                tags.setProperty(MusicColumns::columnNames[MusicColumns::Added], Time::getCurrentTime().toMilliseconds(), nullptr);
                tags.setProperty(MusicColumns::columnNames[MusicColumns::Modified], audioFile.getLastModificationTime().toMilliseconds(), nullptr);
                tags.setProperty(MusicColumns::columnNames[MusicColumns::Location], audioFile.getFullPathName(), nullptr);
                
                tags.setProperty(MusicColumns::columnNames[MusicColumns::TrackNum], int(f.tag()->track()), nullptr);
                
                tags.setProperty(MusicColumns::columnNames[MusicColumns::PlayCount], 0, nullptr);
                     
                return tags;
                
            }
            DBG("FILE COULD NOT BE OPENED");
            return ValueTree();
		}
		else {
            DBG("FILE DOESNT EXIST");
			return ValueTree();
		}

	}
	
    /** Reads the album art metadata for an audio file returning a JUCE image and the image type as a string
     @param audioFile The audio file from which the album art is to be extracted
     @return The album art image and its file type (jpeg or png)
     */
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

    /** Saves album art to an audio file
     @param audioFile The file where the album art is to be saved
     @param newCover The image which is to be saved
     @param imageType The file format of the image
     */
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
    
    /** Takes a JUCE image and converts it to raw data before placing this data in a TagLib::ByteVector, which is a container for a block of raw data
     @param newCover The Image to be read into the ByteVector
     @param imageType The Images type, used to ensure the correct image format is used
     @param fillVector A reference to the ByteVector that will be filled with the image data
     */
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
    
    /** Reads the album art from a file and loads it as data into a MemoryBlock for network transfer
     @param audioFile The audio file from which the album art is to be extracted
     @param fillBlock A reference to the memory block where the album art is going to be read to
     @return The file type of the album art extracted, jpeg or png
     */
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
    
    /** Compares the album art from two files to see if they are the same, used for editing of multiple tracks at once
     @return Whether the two files have the same album art
     */
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
    
    /** Writes tag information to an audio file
     @param columnId The table column which has changed and will be saved to the tag
     @param incomingTrack All of the information about the track
     */
    static void writeTag (int columnId, ValueTree incomingTrack)
    {
        TagLib::FileRef f(incomingTrack.getProperty(MusicColumns::columnNames[MusicColumns::Location]).toString().toUTF8());	
        
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