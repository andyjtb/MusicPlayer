// =============================================================================
#ifndef __COVERFLOW_PLUGIN_XBOX_H__
#define __COVERFLOW_PLUGIN_XBOX_H__
// =============================================================================
#include "includes.h"
// =============================================================================
#include "CoverFlowPlugin.h"
// =============================================================================
class CoverFlowPluginXBox : public CoverFlowPluginInterface
{
	public:
		CoverFlowPluginXBox ();
		// =====================================================================
		void process (bool reflet, 
					  float current, 
					  int coverID, 
					  int selectedID, 
					  int selectedAnim);
		// =====================================================================
		const int getDefaultViewMode ()		{ return 2; }
		const int getDefaultLeftNumber ()	{ return 0; }
		const int getDefaultRightNumber ()	{ return 11; }
		// =====================================================================
	private:
		float _spaceBetweenX;
		float _spaceBetweenZ;
		float _decX;
		float _zoom;
		float _zoomSelected;
};
// =============================================================================
#endif // __COVERFLOW_PLUGIN_XBOX_H__
// =============================================================================