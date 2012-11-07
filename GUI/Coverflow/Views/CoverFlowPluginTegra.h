// =============================================================================
#ifndef __COVERFLOW_PLUGIN_TEGRA_H__
#define __COVERFLOW_PLUGIN_TEGRA_H__
// =============================================================================
#include "includes.h"
// =============================================================================
#include "CoverFlowPlugin.h"
// =============================================================================
class CoverFlowPluginTegra : public CoverFlowPluginInterface
{
	public:
		CoverFlowPluginTegra ();
		// =====================================================================
		void process (bool reflet, 
					  float current, 
					  int coverID, 
					  int selectedID, 
					  int selectedAnim);
		// =====================================================================
		const int getDefaultViewMode ()		{ return 2; }
		const int getDefaultLeftNumber ()	{ return 3; }
		const int getDefaultRightNumber ()	{ return 5; }
		// =====================================================================
	private:
		float _spaceBetween;
		float _radius;
		int   _tilt;
		float _zoom;
		float _zoomSelected;
};
// =============================================================================
#endif // __COVERFLOW_PLUGIN_TEGRA_H__
// =============================================================================