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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "GraphicsPlayButton.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
NewJucerComponent::NewJucerComponent ()
{

    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

NewJucerComponent::~NewJucerComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void NewJucerComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    g.setGradientFill (ColourGradient (Colour (0xffffff),
                                       50.0f, 50.0f,
                                       Colour (0x13000000),
                                       104.0f, 104.0f,
                                       true));
    g.fillEllipse ((float) (-5), (float) (-5), 110.0f, 110.0f);

    g.setGradientFill (ColourGradient (Colour (0xffeaeaea),
                                       50.0f, 50.0f,
                                       Colour (0xff6f6f6f),
                                       48.0f, 120.0f,
                                       false));
    g.fillEllipse (0.0f, 0.0f, 100.0f, 100.0f);

    g.setColour (Colours::black);
    g.drawEllipse (0.0f, 0.0f, 100.0f, 100.0f, 1.0000f);

    g.setGradientFill (ColourGradient (Colours::black,
                                       48.0f, 20.0f,
                                       Colours::black,
                                       48.0f, 104.0f,
                                       false));
    g.fillPath (internalPath1);
    g.setColour (Colours::black);
    g.strokePath (internalPath1, PathStrokeType (0.5000f));

    g.setGradientFill (ColourGradient (Colour (0xffffff),
                                       50.0f, 65.0f,
                                       Colours::white,
                                       88.0f, (float) (-16),
                                       true));
    g.fillEllipse (0.0f, 0.0f, 100.0f, 100.0f);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void NewJucerComponent::resized()
{
    internalPath1.clear();
    internalPath1.startNewSubPath (29.0f, 20.0f);
    internalPath1.lineTo (85.0f, 50.0f);
    internalPath1.lineTo (29.0f, 80.0f);
    internalPath1.closeSubPath();

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="NewJucerComponent" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ffffffff">
    <ELLIPSE pos="-5 -5 110 110" fill=" radial: 50 50, 104 104, 0=ffffff, 1=13000000"
             hasStroke="0"/>
    <ELLIPSE pos="0 0 100 100" fill="linear: 50 50, 48 120, 0=ffeaeaea, 1=ff6f6f6f"
             hasStroke="1" stroke="1, mitered, butt" strokeColour="solid: ff000000"/>
    <PATH pos="0 0 100 100" fill="linear: 48 20, 48 104, 0=ff000000, 1=ff000000"
          hasStroke="1" stroke="0.5, mitered, butt" strokeColour="solid: ff000000"
          nonZeroWinding="1">s 29 20 l 85 50 l 29 80 x</PATH>
    <ELLIPSE pos="0 0 100 100" fill=" radial: 50 65, 88 -16, 0=ffffff, 1=ffffffff"
             hasStroke="0"/>
  </BACKGROUND>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
