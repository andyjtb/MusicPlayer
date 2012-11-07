// =============================================================================
#ifndef __COVERFLOW_CONTEXT_H__
#define __COVERFLOW_CONTEXT_H__
// =============================================================================
#include "includes.h"
#include "singleton.h"
// =============================================================================
#include "CoverFlowOpenGL.h"
// =============================================================================
#define EFFECT_SPEED 20
#define EFFECT_SPEED_SELECTED 15
// =============================================================================
class CoverFlowContext : public Singleton <CoverFlowContext>,
					     public DeletedAtShutdown,
						 public Component,
						 public OpenGLRenderer,
						 private CoverFlowOpenGL,
						 public Thread,
						 public ChangeBroadcaster
{
	public:
		CoverFlowContext ();
		~CoverFlowContext ();
		// =====================================================================
		void start ();
		// =====================================================================
		void setCurrent (const float current);
		void setCurrentSelected (const int current);
		// =====================================================================
		void addBackground (Image* img);
		void addForeground (Image* img);
		// =====================================================================
		const int getSelected () { const ScopedLock lock(cs); return _selected; }
		const float getCurrent () { const ScopedLock lock(cs); return _current; }
		const int getNumber () { const ScopedLock lock(cs); return _itemsArray.size(); }
		// =====================================================================
		void setViewMode (const int m) { const ScopedLock lock(cs); _itemsViewMode = m; }
		const int getViewMode () { const ScopedLock lock(cs); return _itemsViewMode; }
		void setLeftNumber (const int l) { const ScopedLock lock(cs); _leftItemsNumber = l; }
		const int getLeftNumber () { const ScopedLock lock(cs); return _leftItemsNumber; }
		void setRightNumber (const int r) { const ScopedLock lock(cs); _rightItemsNumber = r; }
		const int getRightNumber () { const ScopedLock lock(cs); return _rightItemsNumber; }
		// =====================================================================
		juce_UseDebuggingNewOperator
		// =====================================================================
	private:
		// =====================================================================
		/* inherit of Component */
		void resized ();
		void mouseDown (const MouseEvent& e);
		void mouseDrag (const MouseEvent& e);
		void mouseMove (const MouseEvent& e);
		// =====================================================================
		/* inherit of OpenGLComponent */
		void newOpenGLContextCreated ();
		void renderOpenGL ();
		// =====================================================================
		/* inherit of Thread */
		void run ();
		// =====================================================================
	private:
		// =====================================================================
		/* Thread safe */
		CriticalSection cs;
		// =====================================================================
		double	_ticks;
		double	_ticksSelected;
		GLint	_moveTicks;
		int		_threadLatency;
		// =====================================================================
		/* settings */
		int		_itemsViewMode;
		int		_leftItemsNumber;
		int		_rightItemsNumber;
		// =====================================================================
	private:
		friend class CoverFlowComponent;
		// =====================================================================
		struct CoverFlowItem 
		{
			String	artist;
			String	title;
			Image*	img;

			CoverFlowItem (const String& t, const String& s, Image* i)
				: artist (t), title (s), img (i)
			{}

			~CoverFlowItem ()
			{
				if (img)
					delete img;
			}
		};
		// =====================================================================
		typedef OwnedArray <CoverFlowItem, CriticalSection> CoverFlowItems;
		// =====================================================================
		CoverFlowItems	_itemsArray;
		float			_current;
		int				_selected;
		// =====================================================================
		void addItem (const String& title, const String& subtitle, Image* img);
		CoverFlowItem* getItem (const int index) { return _itemsArray[index]; }
		// =====================================================================
};
// =============================================================================
#endif // __COVERFLOW_CONTEXT_H__
// =============================================================================