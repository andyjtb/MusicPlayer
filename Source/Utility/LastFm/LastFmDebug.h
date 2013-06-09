/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  9 Jun 2013 7:37:37pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_LASTFMDEBUG_LASTFMDEBUG_1FC23D12__
#define __JUCER_HEADER_LASTFMDEBUG_LASTFMDEBUG_1FC23D12__

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class LastFmDebug  : public Component,
                     public ButtonListener
{
public:
    //==============================================================================
    LastFmDebug ();
    ~LastFmDebug();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);



    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    TextButton* md5Button;
    TextEditor* apiKeyText;
    TextEditor* authTokenText;
    TextEditor* secretText;
    TextEditor* md5Editor;
    TextEditor* methodText;
    TextEditor* outputBox;
    TextButton* clearOutput;
    TextButton* authTokenButton;


    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    LastFmDebug (const LastFmDebug&);
    const LastFmDebug& operator= (const LastFmDebug&);
};


#endif   // __JUCER_HEADER_LASTFMDEBUG_LASTFMDEBUG_1FC23D12__
