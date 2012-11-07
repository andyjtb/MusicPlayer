//=============================================================================
#include "CoverFlowPluginCircle.h"
//=============================================================================
CoverFlowPluginCircle::CoverFlowPluginCircle ()
	:	_spaceBetween (22.6f),
		_radius (5.0f),
		_zoomSelected (1.0f)
{
}
//=============================================================================
void CoverFlowPluginCircle::process (bool reflet, 
								     float current, 
								     int coverID, 
								     int selectedID, 
								     int selectedAnim)
{
	double diff;

	// Calcul de l'écart avec l'album courant
	diff = coverID - current;

	// Calcul de la position sur le cercle de l'album
	float angle = diff * _spaceBetween;
	float radian = (angle * 3.14159f / 180);
	float decX = (float)( _radius * sin(radian) );
	float decZ = (float)( _radius * cos(radian) );

	// Positionnement de l'album dans la scene OpenGL
	glTranslatef(0.0f, 0.0f, -4.0f - _radius);
	glTranslatef(decX, 0.0f, decZ);
	glRotatef(angle, 0, 1, 0);

	// Gestion de l'animation de selection
	if ( coverID == selectedID ) 
	{
		glTranslatef ( 0, 
					   0, 
					  (jabs((jabs((float)selectedAnim-50.0f) * 2.0f) - 100.0f) / 100.0f ) * _zoomSelected);
	}
}
//=============================================================================