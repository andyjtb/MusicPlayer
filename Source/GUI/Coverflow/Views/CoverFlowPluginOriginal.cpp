//=============================================================================
#include "CoverFlowPluginOriginal.h"
//=============================================================================
CoverFlowPluginOriginal::CoverFlowPluginOriginal ()
	:	_spaceBetween (2.8f),
		_angle (95),
		_zoom (2.15f),
		_zoomSelected (1.0f)
{
}
//=============================================================================
void CoverFlowPluginOriginal::process (bool reflet, 
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
	glTranslatef (0, 0, -6.0f);
	if ( reflet == false ) 
	{
		glTranslatef (0, 0.05f, 0);
	} else 
	{
		glTranslatef (0, -0.05f, 0);
	}
	glTranslatef (diff+dx/_spaceBetween, 0, 0);
	glTranslatef (0, 0, (1-abs(diff))*_zoom);
	glRotatef	 (-sin(diff)*_angle, 0, 1, 0);

	// Animate the selection
	if ( coverID == selectedID ) 
	{
		glTranslatef ( 0, 
					   0, 
					  (abs((abs((float)selectedAnim-50.0f) * 2.0f) - 100.0f) / 100.0f ) * _zoomSelected );
	}
}
//=============================================================================