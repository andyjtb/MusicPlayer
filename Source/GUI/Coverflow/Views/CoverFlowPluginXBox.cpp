//=============================================================================
#include "CoverFlowPluginXBox.h"
//=============================================================================
CoverFlowPluginXBox::CoverFlowPluginXBox ()
	:	_spaceBetweenX (1.0f),
		_spaceBetweenZ (-1.0f),
		_decX (-1.0f),
		_zoom (2.15f),
		_zoomSelected (1.0f)
{
}
//=============================================================================
void CoverFlowPluginXBox::process (bool reflet, 
								   float current, 
								   int coverID, 
								   int selectedID, 
								   int selectedAnim)
{
   double diff;
   double dx;

   // Distance from current Cover
   dx = coverID - current;
   diff = dx;
   if ( diff < 0 )
      diff = jmax (diff, -1.0);
   else
      diff = jmin (diff,  1.0);

   // Position of Cover inside GL Scene
   glTranslatef (_decX, -0.05f, -4.0f);
   glTranslatef (0, 0.05f, 0);
   glTranslatef (dx/_spaceBetweenX, 0, dx*_spaceBetweenZ);

   // Animate the selection
   if ( coverID == selectedID ) 
   {
	   glTranslatef ( 0, 
					  0, 
					 (jabs((jabs((float)selectedAnim-50.0f) * 2.0f) - 100.0f) / 100.0f ) * _zoomSelected);
   }
}
//=============================================================================