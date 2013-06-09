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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "LastFmDebug.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
LastFmDebug::LastFmDebug ()
    : md5Button (0),
      apiKeyText (0),
      authTokenText (0),
      secretText (0),
      md5Editor (0),
      methodText (0),
      outputBox (0),
      clearOutput (0),
      authTokenButton (0)
{
    addAndMakeVisible (md5Button = new TextButton (String::empty));
    md5Button->setButtonText ("Generate MD5");
    md5Button->addListener (this);

    addAndMakeVisible (apiKeyText = new TextEditor (String::empty));
    apiKeyText->setMultiLine (false);
    apiKeyText->setReturnKeyStartsNewLine (false);
    apiKeyText->setReadOnly (false);
    apiKeyText->setScrollbarsShown (true);
    apiKeyText->setCaretVisible (true);
    apiKeyText->setPopupMenuEnabled (true);
    apiKeyText->setText (String::empty);

    addAndMakeVisible (authTokenText = new TextEditor (String::empty));
    authTokenText->setMultiLine (false);
    authTokenText->setReturnKeyStartsNewLine (false);
    authTokenText->setReadOnly (false);
    authTokenText->setScrollbarsShown (true);
    authTokenText->setCaretVisible (true);
    authTokenText->setPopupMenuEnabled (true);
    authTokenText->setText (String::empty);

    addAndMakeVisible (secretText = new TextEditor (String::empty));
    secretText->setMultiLine (false);
    secretText->setReturnKeyStartsNewLine (false);
    secretText->setReadOnly (false);
    secretText->setScrollbarsShown (true);
    secretText->setCaretVisible (true);
    secretText->setPopupMenuEnabled (true);
    secretText->setText (String::empty);

    addAndMakeVisible (md5Editor = new TextEditor (String::empty));
    md5Editor->setMultiLine (false);
    md5Editor->setReturnKeyStartsNewLine (false);
    md5Editor->setReadOnly (false);
    md5Editor->setScrollbarsShown (true);
    md5Editor->setCaretVisible (true);
    md5Editor->setPopupMenuEnabled (true);
    md5Editor->setText (String::empty);

    addAndMakeVisible (methodText = new TextEditor (String::empty));
    methodText->setMultiLine (false);
    methodText->setReturnKeyStartsNewLine (false);
    methodText->setReadOnly (false);
    methodText->setScrollbarsShown (true);
    methodText->setCaretVisible (true);
    methodText->setPopupMenuEnabled (true);
    methodText->setText (String::empty);

    addAndMakeVisible (outputBox = new TextEditor (String::empty));
    outputBox->setMultiLine (true);
    outputBox->setReturnKeyStartsNewLine (false);
    outputBox->setReadOnly (true);
    outputBox->setScrollbarsShown (true);
    outputBox->setCaretVisible (false);
    outputBox->setPopupMenuEnabled (true);
    outputBox->setText (String::empty);

    addAndMakeVisible (clearOutput = new TextButton (String::empty));
    clearOutput->setButtonText ("Clear");
    clearOutput->addListener (this);

    addAndMakeVisible (authTokenButton = new TextButton (String::empty));
    authTokenButton->setButtonText ("get Auth Token");
    authTokenButton->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 600);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

LastFmDebug::~LastFmDebug()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    deleteAndZero (md5Button);
    deleteAndZero (apiKeyText);
    deleteAndZero (authTokenText);
    deleteAndZero (secretText);
    deleteAndZero (md5Editor);
    deleteAndZero (methodText);
    deleteAndZero (outputBox);
    deleteAndZero (clearOutput);
    deleteAndZero (authTokenButton);


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void LastFmDebug::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    g.setColour (Colours::black);
    g.setFont (Font (15.0000f, Font::plain));
    g.drawText ("Api Key",
                20, 4, 64, 30,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (15.0000f, Font::plain));
    g.drawText ("Auth Token",
                228, 4, 132, 30,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (15.0000f, Font::plain));
    g.drawText ("Secret",
                428, 4, 132, 30,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (15.0000f, Font::plain));
    g.drawText ("MD5 Hash",
                220, 140, 200, 30,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (15.0000f, Font::plain));
    g.drawText ("Method",
                212, 76, 200, 30,
                Justification::centred, true);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void LastFmDebug::resized()
{
    md5Button->setBounds (352, 208, 150, 24);
    apiKeyText->setBounds (24, 32, 150, 24);
    authTokenText->setBounds (224, 32, 150, 24);
    secretText->setBounds (424, 32, 150, 24);
    md5Editor->setBounds (152, 168, 360, 24);
    methodText->setBounds (184, 104, 272, 32);
    outputBox->setBounds (16, 288, 584, 168);
    clearOutput->setBounds (248, 472, 150, 24);
    authTokenButton->setBounds (144, 208, 150, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void LastFmDebug::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == md5Button)
    {
            String apiSigString = "api_key"+apiKeyText->getText()+"method"+methodText->getText()+"token"+authTokenText->getText()+secretText->getText();
        
            DBG(apiSigString);
        
            MD5 apiSig = MD5(apiSigString.toUTF8());//= MD5::fromUTF32(apiSigString);
        
            md5Editor->setText(apiSig.toHexString());
        
        URL sessionAddress = "http://ws.audioscrobbler.com/2.0/?method="+methodText->getText()+"&api_key="+apiKeyText->getText()+"&token="+authTokenText->getText()+"&api_sig="+apiSig.toHexString();
        
            XmlElement* session = sessionAddress.readEntireXmlStream();
            
            if (session != nullptr)
            {
                outputBox->insertTextAtCaret ("\n"+sessionAddress.readEntireTextStream());
                
                String status = session->getAttributeValue(0);
                if (status == "ok")
                {
                    DBG( session->getChildElement(0)->getAllSubText());
                }
                else
                {
                    DBG("Status error " << status);
                }
            }
            
        

    }
    else if (buttonThatWasClicked == clearOutput)
    {
        //[UserButtonCode_clearOutput] -- add your button handler code here..
        //[/UserButtonCode_clearOutput]
    }
    else if (buttonThatWasClicked == authTokenButton)
    {
        URL tokenAddress("http://ws.audioscrobbler.com/2.0/?method=auth.gettoken&api_key="+apiKeyText->getText());
        
        XmlElement* token = tokenAddress.readEntireXmlStream();
        
        if (token != nullptr)
        {
            String status = token->getAttributeValue(0);
            if (status == "ok")
            {
                authTokenText->setText(token->getChildElement(0)->getAllSubText());
                URL userAuth ("http://www.last.fm/api/auth/?api_key="+apiKeyText->getText()+"&token="+authTokenText->getText());
                userAuth.launchInDefaultBrowser();
            }
            else
            {
                //int statusInt = status.getIntValue();
                DBG("Status error " << status);
            }
        }
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="LastFmDebug" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ffffffff">
    <TEXT pos="20 4 64 30" fill="solid: ff000000" hasStroke="0" text="Api Key"
          fontname="Default font" fontsize="15" bold="0" italic="0" justification="36"/>
    <TEXT pos="228 4 132 30" fill="solid: ff000000" hasStroke="0" text="Auth Token"
          fontname="Default font" fontsize="15" bold="0" italic="0" justification="36"/>
    <TEXT pos="428 4 132 30" fill="solid: ff000000" hasStroke="0" text="Secret"
          fontname="Default font" fontsize="15" bold="0" italic="0" justification="36"/>
    <TEXT pos="220 140 200 30" fill="solid: ff000000" hasStroke="0" text="MD5 Hash"
          fontname="Default font" fontsize="15" bold="0" italic="0" justification="36"/>
    <TEXT pos="212 76 200 30" fill="solid: ff000000" hasStroke="0" text="Method"
          fontname="Default font" fontsize="15" bold="0" italic="0" justification="36"/>
  </BACKGROUND>
  <TEXTBUTTON name="" id="35035255e393eb8a" memberName="md5Button" virtualName=""
              explicitFocusOrder="0" pos="352 208 150 24" buttonText="Generate MD5"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTEDITOR name="" id="ad029a957e1e50b6" memberName="apiKeyText" virtualName=""
              explicitFocusOrder="0" pos="24 32 150 24" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTEDITOR name="" id="ba88b4972c8fde32" memberName="authTokenText" virtualName=""
              explicitFocusOrder="0" pos="224 32 150 24" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTEDITOR name="" id="558f63ba881aaf75" memberName="secretText" virtualName=""
              explicitFocusOrder="0" pos="424 32 150 24" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTEDITOR name="" id="841ec925cf7d0b81" memberName="md5Editor" virtualName=""
              explicitFocusOrder="0" pos="152 168 360 24" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTEDITOR name="" id="6085a1b5da0d58c2" memberName="methodText" virtualName=""
              explicitFocusOrder="0" pos="184 104 272 32" initialText="" multiline="0"
              retKeyStartsLine="0" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTEDITOR name="" id="89b2df31bd4f2640" memberName="outputBox" virtualName=""
              explicitFocusOrder="0" pos="16 288 584 168" initialText="" multiline="1"
              retKeyStartsLine="0" readonly="1" scrollbars="1" caret="0" popupmenu="1"/>
  <TEXTBUTTON name="" id="b4d9a2c4a5d2aa4a" memberName="clearOutput" virtualName=""
              explicitFocusOrder="0" pos="248 472 150 24" buttonText="Clear"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="888f79be47a8d4da" memberName="authTokenButton" virtualName=""
              explicitFocusOrder="0" pos="144 208 150 24" buttonText="get Auth Token"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
