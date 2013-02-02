// =============================================================================
#ifndef __COVERFLOW_PLUGIN_CIRCLE_H__
#define __COVERFLOW_PLUGIN_CIRCLE_H__
// =============================================================================
#include "includes.h"
// =============================================================================
#include "CoverFlowPlugin.h"
// =============================================================================
class CoverFlowPluginCircle : public CoverFlowPluginInterface
{
	public:
		CoverFlowPluginCircle ();
		// =====================================================================
		void process (bool reflet, 
					  float current, 
					  int coverID, 
					  int selectedID, 
					  int selectedAnim);
		// =====================================================================
		const int getDefaultViewMode ()		{ return 0; }
		const int getDefaultLeftNumber ()	{ return 3; }
		const int getDefaultRightNumber ()	{ return 4; }
		// =====================================================================
	private:
		float _spaceBetween;
		float _radius;
		float _zoomSelected;
};
// =============================================================================
#endif // __COVERFLOW_PLUGIN_CIRCLE_H__
// =============================================================================