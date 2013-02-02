// =============================================================================
#ifndef __COVERFLOW_PLUGIN_ORIGINAL_H__
#define __COVERFLOW_PLUGIN_ORIGINAL_H__
// =============================================================================
#include "includes.h"
// =============================================================================
#include "CoverFlowPlugin.h"
// =============================================================================
class CoverFlowPluginOriginal : public CoverFlowPluginInterface
{
	public:
		CoverFlowPluginOriginal ();
		// =====================================================================
		void process (bool reflet, 
					  float current, 
					  int coverID, 
					  int selectedID, 
					  int selectedAnim);
		// =====================================================================
		const int getDefaultViewMode ()		{ return 0; }
		const int getDefaultLeftNumber ()	{ return 8; }
		const int getDefaultRightNumber ()	{ return 8; }
		// =====================================================================
	private:
		float _spaceBetween;
		int	  _angle;
		float _zoom;
		float _zoomSelected;
};
// =============================================================================
#endif // __COVERFLOW_PLUGIN_ORIGINAL_H__
// =============================================================================