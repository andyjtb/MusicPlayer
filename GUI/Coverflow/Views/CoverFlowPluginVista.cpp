//=============================================================================
#include "CoverFlowPluginVista.h"
//=============================================================================
CoverFlowPluginVista::CoverFlowPluginVista ()
	:	_spaceBetweenX (1.0f),
		_spaceBetweenY (0.4f),
		_spaceBetweenZ (-1.0f),
		_decX (-1.1f),
		_angleX (-20),
		_zoom (2.15f),
		_zoomSelected (1.0f)
{
}
//=============================================================================
void CoverFlowPluginVista::process (bool reflet, 
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
		diff = jmax (diff,-1.0);
	else
		diff = jmin (diff, 1.0);

	// Position of Cover inside GL Scene
	glTranslatef	(_decX, 0.1f, -4.3f);
	glTranslatef	(dx/_spaceBetweenX, log(dx+1.3f)*_spaceBetweenY, dx*_spaceBetweenZ);
	glRotatef		(_angleX, 0, 1, 0);

	// Animate the selection
	if ( coverID == selectedID ) 
	{
		glTranslatef ( 0, 
					   0, 
					  (jabs((jabs((float)selectedAnim-50.0f) * 2.0f) - 100.0f) / 100.0f ) * _zoomSelected );
	}
}
//=============================================================================