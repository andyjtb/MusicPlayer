//=============================================================================
#include "CoverFlowPlugin.h"
//=============================================================================
#include "CoverFlowPluginCircle.h"
#include "CoverFlowPluginOriginal.h"
#include "CoverFlowPluginPower.h"
#include "CoverFlowPluginTegra.h"
#include "CoverFlowPluginVista.h"
#include "CoverFlowPluginXBox.h"
//=============================================================================
CoverFlowPlugin::CoverFlowPlugin ()
	: _strategy (new CoverFlowPluginOriginal)
{}
//=============================================================================
CoverFlowPlugin::~CoverFlowPlugin ()
{
	delete _strategy;
}
//=============================================================================
StringArray CoverFlowPlugin::getPluginList ()
{
	StringArray result;
	result.add ("Original");
	result.add ("Power");
	result.add ("Tegra");
	result.add ("Vista");
	result.add ("Circle");
	result.add ("XBox");
	return result;
}
//=============================================================================
void CoverFlowPlugin::setPlugin (const int type)
{
	deleteAndZero (_strategy);

	switch (type)
	{
		case 1:
			_strategy = new CoverFlowPluginOriginal ();
			break;
		case 2:
			_strategy = new CoverFlowPluginPower ();
			break;
		case 3:
			_strategy = new CoverFlowPluginTegra ();
			break;
		case 4:
			_strategy = new CoverFlowPluginVista ();
			break;
		case 5:
			_strategy = new CoverFlowPluginCircle ();
			break;
		case 6:
			_strategy = new CoverFlowPluginXBox ();
			break;
		default:
			_strategy = new CoverFlowPluginOriginal ();
			break;
	}
}
//=============================================================================
void CoverFlowPlugin::process (bool reflet, 
							   float current, 
							   int coverID, 
							   int selectedID, 
							   int selectedAnim)
{
	if (_strategy)
		_strategy->process (reflet, current, coverID, selectedID, selectedAnim);
}
//=============================================================================