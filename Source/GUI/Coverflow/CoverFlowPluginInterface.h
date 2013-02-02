// =============================================================================
#ifndef __COVERFLOW_PLUGIN_INTERFACE_H__
#define __COVERFLOW_PLUGIN_INTERFACE_H__
// =============================================================================
#include "includes.h"
//=============================================================================
#ifdef _WIN32
 #include <gl/gl.h>
 #include <gl/glu.h>
#elif defined (LINUX)
 #include <GL/gl.h>
 #include <GL/glut.h>
 #undef KeyPress
#else
 #include <GLUT/glut.h>
#endif
// =============================================================================
class CoverFlowPluginInterface
{
	public:
		// =====================================================================
		/* process */
		virtual void process (bool reflet, 
							  float current, 
							  int coverID, 
							  int selectedID, 
							  int SelectedAnim) = 0;
		// =====================================================================
		/* defaults */
		virtual const int getDefaultViewMode () = 0;
		virtual const int getDefaultLeftNumber () = 0;
		virtual const int getDefaultRightNumber () = 0;
		// =====================================================================
};
// =============================================================================
#endif // __COVERFLOW_PLUGIN_INTERFACE_H__
// =============================================================================