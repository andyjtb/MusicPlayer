/*
  ==============================================================================
  
  This file is part of the dRowAudio JUCE module
  Copyright 2004-12 by dRowAudio.
  
  ------------------------------------------------------------------------------
 
  dRowAudio can be redistributed and/or modified under the terms of the GNU General
  Public License (Version 2), as published by the Free Software Foundation.
  A copy of the license is included in the module distribution, or can be found
  online at www.gnu.org/licenses.
  
  dRowAudio is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
  A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
  
  ==============================================================================
*/

#ifndef __DROWAUDIO_COMPARATORS_H__
#define __DROWAUDIO_COMPARATORS_H__

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
namespace ValueTreeComparators
{
    //==============================================================================
    /** A standard Lexiographc ValueTreeComparitor.
     */
    class Lexicographic
    {
    public:
        Lexicographic (const Identifier attributeToSort_, bool forwards)
        :	attributeToSort (attributeToSort_),
            direction (forwards ? 1 : -1)
        {
        }
        
        int compareElements (const ValueTree &first, const ValueTree &second) const
        {
            int result = 0;
            
            result = first[attributeToSort].toString().compareLexicographically(second[attributeToSort].toString());
            
            return direction * result;
        }
        
    private:
        const Identifier attributeToSort;
        const int direction;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Lexicographic);
    };

    //==============================================================================
    /** A standard Numberical ValueTreeComparitor.
     */
    class Numerical
    {
    public:
        Numerical (const Identifier attributeToSort_, bool forwards)
        :	attributeToSort (attributeToSort_),
            direction (forwards ? 1 : -1)
        {
        }
        
        int compareElements (const ValueTree &first, const ValueTree &second) const
        {
            int result = 0;
            
            result = (double(first[attributeToSort]) > double(second[attributeToSort])) ? 1 : -1;
            
            return direction * result;
        }
        
    private:
        const Identifier attributeToSort;
        const int direction;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Numerical);
    };	
        
    //==============================================================================
    /** A Lexiographc ValueTreeComparitor which will use a second comparator if the
        result is the same, useful when sorting tables withough using the much
        slower maintainSortOrder method.
     */
    class LexicographicWithBackup
    {
    public:
        LexicographicWithBackup (const Identifier attributeToSort_, const Identifier backupAttribute_, bool forwards)
            : attributeToSort (attributeToSort_),
              backupAttribute (backupAttribute_),
              direction (forwards ? 1 : -1)
        {
        }
        
        int compareElements (const ValueTree &first, const ValueTree &second) const
        {
            int result = 0;
            
            result = first[attributeToSort].toString().compareLexicographically (second[attributeToSort].toString());
            
            if (result == 0)
                result = first[backupAttribute].toString().compareLexicographically (second[backupAttribute].toString());
            
            return direction * result;
        }
        
    private:
        const Identifier attributeToSort;
        const Identifier backupAttribute;
        const int direction;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LexicographicWithBackup);
    };
    
    //==============================================================================
    /** Non-dRow - Compare used for sorting by artist/album first then by Lib ID
     */
    class LexicographicWithNumerical
    {
    public:
        LexicographicWithNumerical (const Identifier attributeToSort_, const Identifier backupAttribute_, bool forwards)
        : attributeToSort (attributeToSort_),
        backupAttribute (backupAttribute_),
        direction (forwards ? 1 : -1)
        {
        }
        
        int compareElements (const ValueTree &first, const ValueTree &second) const
        {
            int result = 0;
            
            result = first[attributeToSort].toString().compareLexicographically (second[attributeToSort].toString());
            
            if (result == 0)
                
                
                result = (double(first[backupAttribute]) > double(second[backupAttribute])) ? 1 : -1;
            
            
            return direction * result;
        }
        
    private:
        const Identifier attributeToSort;
        const Identifier backupAttribute;
        const int direction;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LexicographicWithNumerical);
    };
    /** Non-dRow - Compare used for sorting by a string value with track num being a determining factor
     */
    class LexicographicWithTrackNum
    {
    public:
        LexicographicWithTrackNum (const Identifier attributeToSort_, const Identifier backupAttribute_, const Identifier trackNum_, bool forwards)
        : attributeToSort (attributeToSort_),
        backupAttribute (backupAttribute_),
        trackNum (trackNum_),
        direction (forwards ? 1 : -1)
        {
        }
        
        int compareElements (const ValueTree &first, const ValueTree &second) const
        {
            int result = 0;
            
            result = first[attributeToSort].toString().compareLexicographically (second[attributeToSort].toString());
            
            if (result == 0)
                result = first[backupAttribute].toString().compareLexicographically (second[backupAttribute].toString());
            
            if (result == 0)
                result = (double(first[trackNum]) > double(second[trackNum])) ? 1 : -1;
            
            
            return direction * result;
        }
        
    private:
        const Identifier attributeToSort;
        const Identifier backupAttribute;
        const Identifier trackNum;
        const int direction;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LexicographicWithTrackNum);
    };
    

    
} //ValueTreeComparators

namespace XmlComparators
{
    //==============================================================================
    /** An Xml comparator used to sort Lexicographical data.
     */
    class LexicographicSorter
    {
    public:
        LexicographicSorter (const String attributeToSort_, bool forwards)
            : attributeToSort (attributeToSort_),
              direction (forwards ? 1 : -1)
        {
        }

        int compareElements (XmlElement* first, XmlElement* second) const
        {
            int result = first->getStringAttribute (attributeToSort)
                           .compareLexicographically (second->getStringAttribute (attributeToSort));

            if (result == 0)
                result = first->getStringAttribute ("ID")
                           .compareLexicographically (second->getStringAttribute ("ID"));

            return direction * result;
        }

    private:
        const String attributeToSort;
        const int direction;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LexicographicSorter);
    };

    //==============================================================================
    /** An Xml comparator used to sort number data.
     */
    class NumberDataSorter
    {
    public:
        NumberDataSorter (const String attributeToSort_, bool forwards)
        : attributeToSort (attributeToSort_),
          direction (forwards ? 1 : -1)
        {
        }
        
        int compareElements (XmlElement* first, XmlElement* second) const
        {
            if (first->getStringAttribute (attributeToSort).isEmpty()
                || second->getStringAttribute (attributeToSort).isEmpty()) {
                return direction * -1;
            }
            
            int result = ((first->getStringAttribute (attributeToSort).getDoubleValue() 
                           <= (second->getStringAttribute (attributeToSort).getDoubleValue()))
                          ? -1 : 1);
            
            return direction * result;
        }
        
    private:
        const String attributeToSort;
        const int direction;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NumberDataSorter);
    };
} //XmlComparators

#endif  // __DROWAUDIO_COMPARATORS_H__
