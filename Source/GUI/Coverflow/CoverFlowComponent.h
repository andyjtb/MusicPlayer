//=============================================================================
#ifndef __COVERFLOW_COMPONENT_H__
#define __COVERFLOW_COMPONENT_H__
//=============================================================================
#include "includes.h"
// =============================================================================
#include "CoverFlowContext.h"
//==============================================================================
class CoverFlowComponent : public Component,
						   public ComponentListener,
						   private ComboBoxListener,
						   private ButtonListener,
						   private ScrollBarListener,
						   private ChangeListener,
						   private Timer
{
	ComboBox *combobox;
	ScrollBar *scrollbar;
	Image background;

	public:
		CoverFlowComponent ();
		~CoverFlowComponent ();
		//=====================================================================
		void paint (Graphics& g);
		void resized();
		//=====================================================================
	protected:
		void delayedUpdate ();
		void createTemporaryCoverTextures ();
		//=====================================================================
		void componentMovedOrResized (Component &component, 
									  bool wasMoved, bool wasResized);
		//=====================================================================
	private:
		CoverFlowContext* context;
		//=====================================================================
		void timerCallback ();
		bool firstCall;
		//=====================================================================
		void buttonClicked (Button *button);
		void comboBoxChanged (ComboBox *combo);
		void scrollBarMoved (ScrollBar *scrollBarThatHasMoved, 
							 const double currentRangeStart);
		void changeListenerCallback (ChangeBroadcaster *objectThatHasChanged);
		//=====================================================================
};
//=============================================================================
#endif //__COVERFLOW_COMPONENT_H__
//=============================================================================