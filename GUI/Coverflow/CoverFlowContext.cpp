//=============================================================================
#include "CoverFlowContext.h"
//=============================================================================
CoverFlowContext::CoverFlowContext ()
:	Thread ("CoverFlow"),
	CoverFlowOpenGL (this),
	_itemsViewMode (0),
	_leftItemsNumber (5),
	_rightItemsNumber (5),
	_current (4),
	_selected (4),
	_threadLatency (40),
	_moveTicks (0),
	_ticks (0),
	_ticksSelected (0)
{
	setName ("CoverFlow");
    glContext.setRenderer(this);
    glContext.attachTo(*this);
}
//=============================================================================
CoverFlowContext::~CoverFlowContext ()
{
	stopThread (1000);
}
//=============================================================================
void CoverFlowContext::start ()
{
	startThread (5);
}
//=============================================================================
void CoverFlowContext::setCurrentSelected (const int current)
{
	const ScopedLock lock (cs);
	_selected = jlimit (0, _itemsArray.size()-1, current);
}
//=============================================================================
void CoverFlowContext::setCurrent (const float current)
{
	const ScopedLock lock (cs);
	_current = jlimit (0.0f, (float)(_itemsArray.size()-1), current);
}
//=============================================================================
void CoverFlowContext::addBackground (Image img)
{
	const ScopedLock lock (cs);
	GLuint lastID = 0;
	if (_backgroundTexture)
	{
		//lastID = _backgroundTexture->getTextureID();
        lastID = _backgroundTexture->ID;
		delete _backgroundTexture;
		_backgroundTexture = new Texture (loadTexture (img), img.getWidth(), img.getHeight(), true);
		if (lastID != 0)
			glDeleteTextures (1, &lastID);
	}
	else
	{
		_backgroundTexture = new Texture (loadTexture (img), img.getWidth(), img.getHeight(), true);
	}
}
//=============================================================================
void CoverFlowContext::addForeground (Image img)
{
	const ScopedLock lock (cs);
	GLuint lastID = 0;
	if (_foregroundTexture)
	{
		lastID = _foregroundTexture->ID;
		delete _foregroundTexture;
		_foregroundTexture = new Texture (loadTextureAlpha (img), img.getWidth(), img.getHeight(), true);
		if (lastID != 0)
			glDeleteTextures (1, &lastID);
	}
	else
	{
		_foregroundTexture = new Texture (loadTextureAlpha (img), img.getWidth(), img.getHeight(), true);
	}
}
//=============================================================================
void CoverFlowContext::addItem (const String& t, const String& s, Image i)
{
	const ScopedLock lock (cs);
	_itemsArray.add (new CoverFlowItem (t, s, i));
	_itemsArray.minimiseStorageOverheads ();
	_itemsTextures.add (new Texture (loadTexture (i), i.getWidth(), i.getHeight(), true));
	_itemsTextures.minimiseStorageOverheads ();
}
//=============================================================================
void CoverFlowContext::newOpenGLContextCreated ()
{
	initGL (getWidth(), getHeight());
}
//=============================================================================
void CoverFlowContext::renderOpenGL ()
{
	repaintGL ();
}

void CoverFlowContext::openGLContextClosing()
{
    //.. nothing?
}

//=============================================================================
void CoverFlowContext::resized ()
{
	resizeGL (getWidth(), getHeight());
}
//=============================================================================
void CoverFlowContext::run ()
{
	while (!threadShouldExit())
	{
		Thread::sleep (jlimit(10, 100, roundFloatToInt(_threadLatency-(_ticks-_moveTicks))));

		/* MessageManagerLock is absolutly needed here ! 
		   because we call repaint() */
		const MessageManagerLock mmLock (Thread::getCurrentThread());

		if (!mmLock.lockWasGained())
			return; // another thread is trying to kill us;

		_ticks = Time::getMillisecondCounterHiRes ();

		/* refresh the OpenGL context */
		repaint ();

		// Compute distance between current cover and selected
        double dx = _selected - _current;

		if (dx != 0 && (_ticks-_moveTicks) > EFFECT_SPEED)
		{
			// Move the current cursor
			if (jabs(dx) > 0.001) 
			{
				_current += dx / 15;
			} 
			else 
			{
				_current = _selected;
			}

			// Record last tick
			_moveTicks = Time::getMillisecondCounterHiRes ();
		}

		if (_animSelected > 0) 
		{
			if (_ticks - _ticksSelected > EFFECT_SPEED_SELECTED) 
			{
				_animSelected += 5;
               // is the end of animation ?
               if (_animSelected > 100) 
				   _animSelected = 0;
               _ticksSelected = Time::getMillisecondCounterHiRes ();
			}
		}
	}
}
//=============================================================================
void CoverFlowContext::mouseMove (const MouseEvent& e)
{
	// Picking position
	_pickingX = e.x;
	_pickingY = e.y;
	
	pickingGL ();
}
//=============================================================================
void CoverFlowContext::mouseDown (const MouseEvent& e)
{
	// Picking position
	_pickingX = e.x;
	_pickingY = e.y;

	pickingGL ();

	if (listPicking.size() > 0)
	{
		if (_selected == listPicking.getFirst()->id)
		{
			_animSelected = 5;

			// open (selected);
			// play (selected);
			// 
			// use SHIFT or CTRL to manage both Keypress and Mouse
			// anything you want, this is a principle. 
		}
		else
		{
			_selected = listPicking.getFirst()->id; /*TODO*/
			
			/* send a broadcast message for scrollbar sync */
			sendChangeMessage ();
		}
	}
}
//=============================================================================
void CoverFlowContext::mouseDrag (const MouseEvent& e)
{
	const float mouseDelta = e.getDistanceFromDragStartX() / 100.0f;
}
//=============================================================================