//==============================================================================
#ifndef __INCLUDES_H__
#define __INCLUDES_H__
//==============================================================================
#define JUCE_GIT 1
//==============================================================================
#define DONT_LIST_JUCE_AUTOLINKEDLIBS 1
//==============================================================================
#ifdef WIN32
//==============================================================================
#include <windows.h>
#include "juce.h"
//==============================================================================
#pragma warning(disable: 4244) // 'conversion' conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable: 4275) // non - DLL-interface classkey 'identifier' used as base for DLL-interface classkey 'identifier'
#pragma warning(disable: 4514) // unreferenced inline/local function has been removed
#pragma warning(disable: 4800) // 'type' : forcing value to bool 'true' or 'false' (performance warning)
#pragma warning(disable: 4097) // typedef-name 'identifier1' used as synonym for class-name 'identifier2'
#pragma warning(disable: 4706) // assignment within conditional expression
#pragma warning(disable: 4786) // truncating debug info after 255 characters
#pragma warning(disable: 4660) // template-class specialization 'identifier' is already instantiated
#pragma warning(disable: 4355) // 'this' : used in base member initializer list
#pragma warning(disable: 4231) // nonstandard extension used : 'extern' before template explicit instantiation
#pragma warning(disable: 4710) // function not inlined
#pragma warning(disable: 4482) // extension non standard utilis»e
#pragma warning(disable: 4172) // retourne l'adresse d'une variable locale ou temporaire
//==============================================================================
#else
//==============================================================================
#include "../JuceLibraryCode/JuceHeader.h"
//==============================================================================
#endif
//==============================================================================
#include "binary.h"
//==============================================================================
template <typename T>
inline T jabs (T value) { if (value < 0) return -value; else return value; }
//==============================================================================
//static void drawTextWithBorder (Graphics &g, 
//								Font* font,
//								const String& text,
//								const Justification& justification,
//								const Rectangle& bounds, 
//								const Colour& solidColour, 
//								const Colour& strokeColour, 
//								const float strokeSize)
//{
//	GlyphArrangement glyphs;
//	glyphs.addFittedText (*font, 
//						  text, 
//						  bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(), 
//						  justification, 1, 1.0);
//	Path path;
//	glyphs.createPath (path);
//
//	g.saveState ();
//	g.setColour (strokeColour);
//	g.strokePath (path, PathStrokeType(strokeSize));
//
//	g.setColour (solidColour);
//	g.fillPath (path);
//	g.restoreState ();
//}
//==============================================================================
#endif //__INCLUDES_H__
//==============================================================================