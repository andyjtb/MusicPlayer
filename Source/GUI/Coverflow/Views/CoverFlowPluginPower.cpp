//=============================================================================
#include "CoverFlowPluginPower.h"
//=============================================================================
CoverFlowPluginPower::CoverFlowPluginPower ()
	:	_spaceBetween (10.0f),
		_spaceCenter (12.0f),
		_spaceZ (0.1f),
		_radius (5.0f),
		_zoom (5.1f),
		_zoomSelected (1.0f)
{
}
//=============================================================================
void CoverFlowPluginPower::process (bool reflet, 
								    float current, 
								    int coverID, 
								    int selectedID, 
								    int selectedAnim)
{
	double dx;
	double diff;
	double diff2;

	// Distance from current Cover
	dx = coverID - current;
	if ( dx > 0 ) 
	{
		diff2 = jmin(dx,  1.0);
	} 
	else 
	{
		diff2 = jmax(dx, -1.0);
	}
	diff = jmin(jabs(dx), 0.5) * 2;

	// Compute position around the circle
	float angle = 180 + (dx * _spaceBetween) + (diff2 * _spaceCenter);
	float radian = (angle * 3.14159f / 180);
	float decX = (float)( _radius * sin(radian) );
	float decZ = (float)( _radius * cos(radian) );

	// Position of Cover inside GL Scene
	glTranslatef (0.0f, 0.0f, -_radius);
	glTranslatef (-decX, 0.0f, decZ);
	glRotatef	 (180-angle, 0, 1, 0);
	glTranslatef (0, 0, (1-diff)*_zoom);
	glTranslatef (0, 0, jabs(dx)*(-_spaceZ));

	// Animate the selection
	if ( coverID == selectedID ) 
	{
	  glTranslatef ( 0, 
					 0, 
					(jabs((jabs((float)selectedAnim-50.0f) * 2.0f) - 100.0f) / 100.0f ) * _zoomSelected);
	}
}
//=============================================================================