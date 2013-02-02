//=============================================================================
#include "CoverFlowPluginTegra.h"
//=============================================================================
CoverFlowPluginTegra::CoverFlowPluginTegra ()
	:	_spaceBetween (10.0f),
		_radius (8.0f),
		_tilt (30),
		_zoom (0.3f),
		_zoomSelected (1.0f)
{
}
//=============================================================================
void CoverFlowPluginTegra::process (bool reflet, 
								    float current, 
								    int coverID, 
								    int selectedID, 
								    int selectedAnim)
{
	double diff;
	double dx;

	// Distance from current Cover
	dx = coverID - current;
	diff = jmin (jabs(dx), 1.0);

	// Calcul de la position sur le cercle de l'album
	float angle = dx * _spaceBetween;
	float radian = (angle * 3.14159f / 180);
	float decX = (float)( _radius * sin(radian) );
	float decZ = (float)( _radius * cos(radian) );

	// Position of Cover inside GL Scene
	glTranslatef (-0.5f, 0.0f, -5.0f - _radius);
	glTranslatef (decX, 0.0f, decZ);
	glRotatef	 (-_tilt, 0, 1, 0);
	glTranslatef ((1-diff)*_zoom*2, 0, (1-diff)*_zoom);

	// Animate the selection
	if ( coverID == selectedID ) 
	{
		glTranslatef ( (jabs((jabs((float)selectedAnim-50.0f) * 2.0f) - 100.0f) / 100.0f )* 2.0f * _zoomSelected, 
						0, 
					   (jabs((jabs((float)selectedAnim-50.0f) * 2.0f) - 100.0f) / 100.0f ) * _zoomSelected );
	}
}
//=============================================================================