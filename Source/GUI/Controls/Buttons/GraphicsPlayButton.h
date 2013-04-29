/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  29 Apr 2013 5:59:08pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_NEWJUCERCOMPONENT_GRAPHICSPLAYBUTTON_AA8C1A01__
#define __JUCER_HEADER_NEWJUCERCOMPONENT_GRAPHICSPLAYBUTTON_AA8C1A01__

//[Headers]     -- You can add your own extra header files here --
#include "juce.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class NewJucerComponent  : public Component
{
public:
    //==============================================================================
    NewJucerComponent ();
    ~NewJucerComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();



    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    Path internalPath1;


    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    NewJucerComponent (const NewJucerComponent&);
    const NewJucerComponent& operator= (const NewJucerComponent&);
};


#endif   // __JUCER_HEADER_NEWJUCERCOMPONENT_GRAPHICSPLAYBUTTON_AA8C1A01__
