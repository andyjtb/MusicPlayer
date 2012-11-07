// =============================================================================
#ifndef __COVERFLOW_PLUGIN_POWER_H__
#define __COVERFLOW_PLUGIN_POWER_H__
// =============================================================================
#include "includes.h"
// =============================================================================
#include "CoverFlowPlugin.h"
// =============================================================================
class CoverFlowPluginPower : public CoverFlowPluginInterface
{
	public:
		CoverFlowPluginPower ();
		// =====================================================================
		void process (bool reflet, 
					  float current, 
					  int coverID, 
					  int selectedID, 
					  int selectedAnim);
		// =====================================================================
		const int getDefaultViewMode ()		{ return 0; }
		const int getDefaultLeftNumber ()	{ return 6; }
		const int getDefaultRightNumber ()	{ return 7; }
		// =====================================================================
	private:
		float _spaceBetween;
		float _spaceCenter;
		float _spaceZ;
		float _radius;
		float _zoom;
		float _zoomSelected;
};
// =============================================================================
#endif // __COVERFLOW_PLUGIN_POWER_H__
// =============================================================================