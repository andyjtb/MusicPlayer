//
//  LibraryTreeView.h
//  MusicPlayer
//
//  Created by Andy on 02/02/2013.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef LIBRARYTREEVIEW
#define LIBRARYTREEVIEW  

#include "../JuceLibraryCode/JuceHeader.h"
#include "Settings.h"

//==============================================================================
class LibraryTreeViewItem  : public TreeViewItem
{
public:
    LibraryTreeViewItem (XmlElement& xml_)
    : xml (xml_)
    {
    }
    
    int getItemWidth() const
    {
        return xml.getIntAttribute ("width", -1);
    }
    
    String getUniqueName() const
    {
        return xml.getTagName();
    }
    
    bool mightContainSubItems()
    {
        return xml.getFirstChildElement() != 0;
    }
    
    void paintItem (Graphics& g, int width, int height)
    {
        // if this item is selected, fill it with a background colour..
        if (isSelected())
            g.fillAll (Colours::blue.withAlpha (0.3f));
        
        // use a "colour" attribute in the xml tag for this node to set the text colour..
        //g.setColour (Colour (xml.getStringAttribute ("colour", "ff000000").getHexValue32()));
        
        g.setFont (height * 0.7f);
        
        // draw the xml element's tag name..
        g.drawText (xml.getTagName(),
                    4, 0, width - 4, height,
                    Justification::centredLeft, true);
    }
    
    void itemOpennessChanged (bool isNowOpen)
    {
        if (isNowOpen)
        {
            // if we've not already done so, we'll now add the tree's sub-items. You could
            // also choose to delete the existing ones and refresh them if that's more suitable
            // in your app.
            if (getNumSubItems() == 0)
            {
                // create and add sub-items to this node of the tree, corresponding to
                // each sub-element in the XML..
                
                forEachXmlChildElement (xml, child)
                {
                    jassert (child != 0);
                    addSubItem (new LibraryTreeViewItem (*child));
                }
            }
        }
        else
        {
            // in this case, we'll leave any sub-items in the tree when the node gets closed,
            // though you could choose to delete them if that's more appropriate for
            // your application.
        }
    }    
private:
    XmlElement& xml;
};


class LibraryTreeView : public Component
{
public:
    LibraryTreeView();
    ~LibraryTreeView();
    
//    void paint (Graphics& g);
    void resized();
    
private:
    
    ScopedPointer<TreeViewItem> rootItem;
    ScopedPointer<XmlElement> libraryXml;
    ValueTree playlistsValueTree;
    
    TreeView treeView;
};

#endif
