// =============================================================================
#ifndef __COVERFLOW_PLUGIN_VISTA_H__
#define __COVERFLOW_PLUGIN_VISTA_H__
// =============================================================================
#include "includes.h"
// =============================================================================
#include "CoverFlowPlugin.h"
// =============================================================================
class CoverFlowPluginVista : public CoverFlowPluginInterface
{
	public:
		CoverFlowPluginVista ();
		// =====================================================================
		void process (bool reflet, 
					  float current, 
					  int coverID, 
					  int selectedID, 
					  int selectedAnim);
		// =====================================================================
		const int getDefaultViewMode ()		{ return 2; }
		const int getDefaultLeftNumber ()	{ return 0; }
		const int getDefaultRightNumber ()	{ return 12; }
		// =====================================================================
	private:
		float _spaceBetweenX;
		float _spaceBetweenY;
		float _spaceBetweenZ;
		float _decX;
		int   _angleX;
		float _zoom;
		float _zoomSelected;
};
// =============================================================================
#endif // __COVERFLOW_PLUGIN_VISTA_H__
// =============================================================================