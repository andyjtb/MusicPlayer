// =============================================================================
#ifndef __COVERFLOW_PLUGIN_H__
#define __COVERFLOW_PLUGIN_H__
// =============================================================================
#include "includes.h"
// =============================================================================
#include "CoverFlowPluginInterface.h"
// =============================================================================
class CoverFlowPlugin
{
	public:
		CoverFlowPlugin ();
		~CoverFlowPlugin ();
		// =====================================================================
		StringArray getPluginList ();
		void setPlugin (const int type);
		// =====================================================================
		/* process */
		void process (bool reflet, 
					  float current, 
					  int coverID, 
					  int selectedID, 
					  int selectedAnim);
		// =====================================================================
		/* defaults */
		const int getDefaultViewMode ()		{ return _strategy->getDefaultViewMode (); }
		const int getDefaultLeftNumber ()	{ return _strategy->getDefaultLeftNumber (); }
		const int getDefaultRightNumber ()	{ return _strategy->getDefaultRightNumber (); }
		// =====================================================================
	private:
		CoverFlowPluginInterface* _strategy;
};
// =============================================================================
#endif // __COVERFLOW_PLUGIN_H__
// =============================================================================