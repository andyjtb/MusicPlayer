//=============================================================================
#include "CoverFlowOpenGL.h"
//=============================================================================
#include "CoverFlowContext.h"
//=============================================================================
// Lighting default parameters
GLfloat material_specular []	= { 1.0, 1.0, 1.0, 1.0 };
GLfloat material_shininess []	= { 50.0 };
GLfloat light_positions []		= { 0.0, 0.0, 5.0, 0.0 };
//=============================================================================
CoverFlowOpenGL::CoverFlowOpenGL (CoverFlowContext* p)
:	_currentWidth (0),
	_currentHeight (0),
	_sceneZoom (0.25f),
	_verticalMove (0.0f),
	_horizontaMove (0.2f),
	_verticalAngle (0),
	_horizontalAngle (0),
	_reflectionActive (true),
	_reflectionHeight (35),
	_keepAspectRatio (true),
	_backgroundTexture (0),
	_foregroundTexture (0),
	_animSelected (0),
	_backColour (Colours::black),
	_tableColour (Colours::black),
	_pickingX (0),
	_pickingY (0),
	 parent (p)
{
}
//=============================================================================
CoverFlowOpenGL::~CoverFlowOpenGL ()
{
	if (_backgroundTexture)
	{
		glDeleteTextures (1, &_backgroundTexture->ID);
		delete _backgroundTexture;
	}

	for (int i = 0; i < _itemsTextures.size(); i++)
	{
		glDeleteTextures (1, &_itemsTextures[i]->ID);
		_itemsTextures.remove (i, true);
	}
}
//=============================================================================
void CoverFlowOpenGL::initGL (const int newWidth, const int newHeight)
{
	//basic
	glClearColor (0.0f, 0.0f, 0.0f, 0.0f);              // Erase color definition
	glShadeModel (GL_SMOOTH);                           // Enable Smooth Shading
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Really Nice Perspective Calculations

	//z-buffer
	glDepthFunc (GL_LEQUAL);                            // The Type Of Depth Testing To Do
	glClearDepth (1.0f);                                // Depth Buffer Setup

	//init light0
	glEnable (GL_LIGHT0);
	glMaterialfv (GL_FRONT, GL_SPECULAR, material_specular);
	glMaterialfv (GL_FRONT, GL_SHININESS, material_shininess);
	glLightfv (GL_LIGHT0, GL_POSITION, light_positions);

	//init plugin
	parent->setViewMode (plugin.getDefaultViewMode());
	parent->setLeftNumber (plugin.getDefaultLeftNumber());
	parent->setRightNumber (plugin.getDefaultRightNumber());

	resizeGL (newWidth, newHeight);
}
//=============================================================================
void CoverFlowOpenGL::resizeGL (const int newWidth, const int newHeight)
{
	_currentWidth = newWidth;
	_currentHeight = newHeight;

	// Redefinition de la fenêtre
	glViewport (0, 0, _currentWidth, _currentHeight);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();

	// Modification de la perspective
	gluPerspective (45.0, (float)_currentWidth/(float)_currentHeight, 1.0, 300.0);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
}
//=============================================================================
void CoverFlowOpenGL::repaintGL ()
{
	// Efface le frame buffer et le Z-buffer
	glClear (GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);

	// Choisit la matrice MODELVIEW
	glMatrixMode (GL_MODELVIEW);

	// Réinitialise la matrice
	glLoadIdentity ();

	drawBackground ();

	if (parent->getNumber () == 0)
		return;

	// Scene positions
	glTranslatef (_horizontaMove, _verticalMove, _sceneZoom);
	glRotatef (_horizontalAngle, 0, 1, 0);
	glRotatef (_verticalAngle,   1, 0, 0);

	// Draw the cover reflect
	if (_reflectionHeight > 0) 
	{
		glPushMatrix ();
		glScalef (1.0f, -1.0f, 1.0f);
		glTranslatef (0.0f, 2.0f, 0.0f);
		viewItems (true, false);
		glPopMatrix ();
	}

	// Draw the covers
	viewItems (false, false);

	drawForeground ();
}
//=============================================================================
void CoverFlowOpenGL::pickingGL ()
{
	GLint  hits=0;

	// Save the OpenGL attributs
	glPushAttrib (GL_ALL_ATTRIB_BITS);

	// Get the OpenGL environment
	GLint viewport[4];
	glGetIntegerv (GL_VIEWPORT, viewport);

	// Define the Picking buffer
	GLuint selectBuf[BUFSIZE];
	glSelectBuffer (BUFSIZE, selectBuf);

	// Activate Picking mode
	glRenderMode (GL_SELECT);

	// GL_PROJECTION
	glMatrixMode (GL_PROJECTION);
	glPushMatrix ();
	glLoadIdentity();

	// Init the Picking zone
	gluPickMatrix ((GLdouble) _pickingX, (GLdouble) (viewport[3] - _pickingY), 2.0, 2.0, viewport);

	// Init the point of view
	gluPerspective (45.0, (float)_currentWidth/(float)_currentHeight, 1.0, 300.0);

	// Clean the Frame Buffer and the Z-Order Buffer
	glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
	glClear (GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);

	// GL_MODELVIEW
	glMatrixMode (GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// Init the Picking informations
	glInitNames();
	glPushName(0);

	// General positions
	glTranslatef (_horizontaMove, _verticalMove, _sceneZoom);
	glRotatef (_horizontalAngle, 0, 1, 0);
	glRotatef (_verticalAngle,   1, 0, 0);

	// Draw the Covers in Picking mode
	viewItems (false, true);

	// Record the Picking
	glPopName ();

	// Re-init the GL Scene
	glPopMatrix ();
	glMatrixMode (GL_PROJECTION);
	glPopMatrix ();

	// Get the Picking informations
	hits = glRenderMode (GL_RENDER);
	processHits (hits, selectBuf, listPicking);

	// Re-init the GL attributes
	glPopAttrib();
}
//=============================================================================
void CoverFlowOpenGL::processHits (GLint hits, 
								   GLuint buffer[], 
								   OwnedArray<PickingInfo>& list)
{
   int i; // count
   unsigned int closest=~0; // distance
   int id=0; // name

   list.clear (true);

   // Check the hits
   if (hits==0) 
	   return;

   // Store each hits in the list before sorting
   for (i=0; i<hits; ++i) 
   {
      PickingInfo* info = new PickingInfo();

	  id = buffer[(i<<2)+3];
	  closest = buffer[(i<<2)+1];

	  info->id = id;
	  info->z = closest;
	  list.add (info);
   }
   // Sort the hits by Z-Order
   list.sort (sorter);
}
//=============================================================================
void CoverFlowOpenGL::enable2D ()
{
	// Switch to projection mode
	glMatrixMode (GL_PROJECTION);

	// Save previous
	glPushMatrix ();
	glLoadIdentity ();
	gluOrtho2D (0, _currentWidth, 0, _currentHeight);

	// Invert the y axis, down is positive
	glScalef (1, -1, 1);

	// Move the origin to the upper left corner
	glTranslatef (0, -_currentHeight, 0);
	glMatrixMode (GL_MODELVIEW);

	glPushMatrix ();
	glLoadIdentity ();

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable (GL_TEXTURE_2D);
}
//=============================================================================
void CoverFlowOpenGL::disable2D ()
{
	glDisable (GL_TEXTURE_2D);
	glDisable (GL_BLEND);

	glPopMatrix ();

	glMatrixMode (GL_PROJECTION);
	glPopMatrix ();
	glMatrixMode (GL_MODELVIEW);
}
//=============================================================================
void CoverFlowOpenGL::drawForeground ()
{
	glPushMatrix ();
	glScalef (1.0, 1.0, 1.0);
	enable2D ();

	// Draw the foreground texture
	if (_foregroundTexture)
	{
		glBindTexture (GL_TEXTURE_2D, _foregroundTexture->ID);
		glColor4f (1, 1, 1, 1);
		glBegin (GL_QUADS);
			glTexCoord2f (0,0); glVertex2f (0, 0);
			glTexCoord2f (0,1); glVertex2f (0, _foregroundTexture->ID);
			glTexCoord2f (1,1); glVertex2f (_foregroundTexture->width, _foregroundTexture->height);
			glTexCoord2f (1,0); glVertex2f (_foregroundTexture->width, 0);
		glEnd ();
	}

	disable2D ();
	glPopMatrix ();
}
//=============================================================================
void CoverFlowOpenGL::drawBackground ()
{
	glPushMatrix ();
	glScalef (1.0, 1.0, 1.0);
	enable2D ();

	// Trace le fond d'ecran à partir d'une texture
	if (_backgroundTexture)
	{
		glBindTexture (GL_TEXTURE_2D, _backgroundTexture->ID);
		glColor4f (1, 1, 1, 1);
		glBegin (GL_QUADS);
			glTexCoord2f (0,0); glVertex2f (0, 0);
			glTexCoord2f (0,1); glVertex2f (0, _currentHeight);
			glTexCoord2f (1,1); glVertex2f (_currentWidth, _currentHeight);
			glTexCoord2f (1,0); glVertex2f (_currentWidth, 0);
		glEnd ();
	}
	else
	{
		// sinon dessine un fond d'ecran
		glDisable (GL_TEXTURE_2D);

		// Dessin du fond
		glColor4f (_backColour.getRed()/255, _backColour.getGreen()/255, _backColour.getBlue()/255, 1);
		glBegin (GL_QUADS);
			glTexCoord2f (0,0); glVertex2f (0, 0);
			glTexCoord2f (0,1); glVertex2f (0, _currentHeight);
			glTexCoord2f (1,1); glVertex2f (_currentWidth, _currentHeight);
			glTexCoord2f (1,0); glVertex2f (_currentWidth, 0);
		glEnd ();

		// Dessin du degrade
		glBegin (GL_QUADS);
			glColor4f (_tableColour.getRed()/255, _tableColour.getGreen()/255, _tableColour.getBlue()/255, 0);
			glTexCoord2f (1,0); glVertex2f (_currentWidth, _currentHeight/2);
			glTexCoord2f (0,0); glVertex2f (0, _currentHeight/2);
			glColor4f (_tableColour.getRed()/255, _tableColour.getGreen()/255, _tableColour.getBlue()/255, 1);
			glTexCoord2f (0,1); glVertex2f (0, _currentHeight/2+20);
			glTexCoord2f (1,1); glVertex2f (_currentWidth, _currentHeight/2+20);
		glEnd ();

		// Dessin du sol transparent
		glColor4f (_tableColour.getRed()/255, _tableColour.getGreen()/255, _tableColour.getBlue()/255, 0.5);
		glBegin (GL_POLYGON);
			glVertex3f	( 2.5f, -1.0f,  2.0f);
			glVertex3f	(-2.5f, -1.0f,  2.0f);
			glVertex3f	(-2.5f, -1.0f, -2.0f);
			glVertex3f	( 2.5f, -1.0f, -2.0f);
		glEnd();

		glEnable (GL_TEXTURE_2D);
	}

	disable2D ();
	glPopMatrix ();
}
//=============================================================================
void CoverFlowOpenGL::viewItems (bool reflection = false,
								 bool picking = false)
{
	const int mode = parent->getViewMode();
	const float current = parent->getCurrent ();
	const int items = parent->getNumber ();
	const int left = parent->getLeftNumber ();
	const int right = parent->getRightNumber ();

	// Left to Right mode
	if (mode == 1)
	{
		// Change the draw order in Picking mode
		if (picking)
		{
			int i = jmin (int(current)+1+right, items)-1;
			int max = jmax (int(current)-left, 0);
			for (; i >= max; --i)
				drawItems (i, reflection, picking);
		}
		else
		{
			int i = jmax (int(current)-left, 0);
			int max = jmin (int(current)+1+right, items)-1;
			for (; i <= max; ++i)
				drawItems (i, reflection, picking);
		}
	}
	else
	// Right to Left mode
	if (mode == 2)
	{
		// Change the draw order in Picking mode
		if (picking)
		{
			int i = jmax (int(current)-left, 0);
			int max = jmin (int(current)+1+right, items)-1;
			for (; i <= max; ++i)
				drawItems (i, reflection, picking);
		}
		else
		{
			int i = jmin (int(current)+1+right, items)-1;
			int max = jmax (int(current)-left, 0);
			for (; i >= max; --i)
				drawItems (i, reflection, picking);
		}
	}
	else
	// Right->Left->Center mode
	{
		int i, max;
		int center = (int)current;

		// Center repositionning
		if ((current - (float)center) > 0.5f) 
			center = (int)current + 1;

		// Inverse displaying order in Picking mode
		if (picking)
		{
			// Display center cover
			drawItems (center, reflection, picking);
			// Display right covers
			i = center+1;
			max = jmin (int(current)+1+right, items)-1;
			for (; i <= max; ++i)
				drawItems (i, reflection, picking);
			// Display left covers
			i = center-1;
			max = jmax (int(current)-left, 0);
			for (; i >= max; --i)
				drawItems (i, reflection, picking);
		}
		else
		{
			// Display right covers
			i = jmin (int(current)+1+right, items)-1;
			max = center;
			for (; i > max; --i)
				drawItems (i, reflection, picking);
			// Display left covers
			i = jmax (int(current)-left, 0);
			max = center;
			for (; i < max; ++i)
				drawItems (i, reflection, picking);
			// Display center cover
			drawItems (center, reflection, picking);
		}
	}
}
//=============================================================================
void CoverFlowOpenGL::drawItems (int index, 
								 bool reflection = false, 
								 bool picking = false)
{
	const int items = parent->getNumber ();
	if (items <= 0)
		return;

	const int selected = parent->getSelected ();
	const float current = parent->getCurrent ();
	const int left = parent->getLeftNumber ();
	const int right = parent->getRightNumber ();

	float alpha = 1.0f;
	float textureWidth;
	float textureHeight;

	// Cover fade loading
	if (index == jmax(int(current)-left, 0) && 
		index-1 < int(current)-left)
		alpha = jabs (1 - jabs (current - int(current)));
	else
	if ((index+1) == jmin (int(current)+1+right, items) &&
		index+1 > int(current)+right)
		alpha = jabs (current - int(current));
	else
		alpha = 1.0f;

	// Load the stack
	glPushMatrix();

	// Call the plugin function
	plugin.process (reflection, current, index, selected, _animSelected);

	// Check Picking
	if (!picking)
	{
		// Cover loading
		if (_itemsTextures[index]->ID != 0)
		{
            glBindTexture (GL_TEXTURE_2D, _itemsTextures[index]->ID);
            textureWidth  = float(_itemsTextures[index]->width);
            textureHeight = float(_itemsTextures[index]->height);
		}
		else
		{
			// Cover not yet loaded
            glBindTexture (GL_TEXTURE_2D, _itemsTextures[0]->ID);
            textureWidth  = float(_itemsTextures[0]->width);
            textureHeight = float(_itemsTextures[0]->height);
		}
	}
	else
	{
		// Cover loading
		if (_itemsTextures[index]->ID != 0)
		{
			textureWidth  = float(_itemsTextures[index]->width);
			textureHeight = float(_itemsTextures[index]->height);
		} 
		else 
		{
			// Cover not yet loaded
			textureWidth  = float(_itemsTextures[0]->width);
			textureHeight = float(_itemsTextures[0]->height);
		}
		// Record the cover ID
		glLoadName (index);
	}

	if (!_keepAspectRatio)
	{
		if ((float(textureWidth)/float(textureHeight)) < 0.92f) 
		{
			// DVD ratio
			textureWidth  = 1.8f;
			textureHeight = 2.4f;
		} 
		else 
		{
			// CD ratio
			textureWidth  = 2.0f;
			textureHeight = 2.0f;
		}
	}

	// Draw cover
	if (picking)
	{
		if (alpha > 0.5f)
			drawItem (textureWidth, textureHeight, reflection, alpha, index);
	}
	else
		drawItem (textureWidth, textureHeight, reflection, alpha, index);

	// Unload the stack
	glPopMatrix();
}
//=============================================================================
void CoverFlowOpenGL::drawItem (float textureWidth, 
								float textureHeight,
								bool reflection = false,
								float alpha = 1.0f,
								int index = 0)
{
	float maxWidth = 2.0f;
	float maxHeight = 2.4f;
	float destinationWidth;
	float destinationHeight;

	// Déterminer le sens de l'image (vertical/horizontal)
	destinationWidth = (maxHeight * textureWidth) / textureHeight;
	if (destinationWidth > maxWidth) 
	{
		destinationWidth = maxWidth;
		destinationHeight = (maxWidth * textureHeight) / textureWidth;
	}
	else
		destinationHeight = maxHeight;

	glPushMatrix();

	glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable (GL_DEPTH_TEST);

	if (!reflection)
	{
		glEnable (GL_TEXTURE_2D);

		// Item Rectangle
		glColor4f (1, 1, 1, alpha); //blanc
		glBegin (GL_QUADS);
			glTexCoord2f (0.0f, 1.0f); glVertex3f (-(destinationWidth/2.0f), -1.0f, 0);
			glTexCoord2f (1.0f, 1.0f); glVertex3f ( (destinationWidth/2.0f), -1.0f, 0);
			glTexCoord2f (1.0f, 0.0f); glVertex3f ( (destinationWidth/2.0f), (destinationHeight-1.0f), 0);
			glTexCoord2f (0.0f, 0.0f); glVertex3f (-(destinationWidth/2.0f), (destinationHeight-1.0f), 0);
		glEnd ();

		glDisable (GL_TEXTURE_2D);
	}
	else
	{
		if (_reflectionActive)
		{
			// reflet de la pochette
			glEnable (GL_TEXTURE_2D);
			glBegin (GL_QUADS);
				glColor4f	 (1, 1, 1, alpha/2);
				glTexCoord2f (0.0f, 1.0f); glVertex3f (-(destinationWidth/2.0f), -1.0f, 0);
				glTexCoord2f (1.0f, 1.0f); glVertex3f ( (destinationWidth/2.0f), -1.0f, 0);
				glColor4f	 (1, 1, 1, 0);
				glTexCoord2f (1.0f, 1.0f-((float)_reflectionHeight/100.0f)); glVertex3f( (destinationWidth/2.0f), (destinationHeight*((float)_reflectionHeight/100.0f))-1.0f, 0);
				glTexCoord2f (0.0f, 1.0f-((float)_reflectionHeight/100.0f)); glVertex3f(-(destinationWidth/2.0f), (destinationHeight*((float)_reflectionHeight/100.0f))-1.0f, 0);
			glEnd ();
			glDisable (GL_TEXTURE_2D);
		}
		else
		{
			// reflet de la pochette
			glEnable (GL_TEXTURE_2D);
			glColor4f (1, 1, 1, alpha);
			glBegin (GL_QUADS);
				glTexCoord2f (0.0f, 1.0f); glVertex3f (-(destinationWidth/2.0f), -1.0f, 0);
				glTexCoord2f (1.0f, 1.0f); glVertex3f ( (destinationWidth/2.0f), -1.0f, 0);
				glTexCoord2f (1.0f, 1.0f-((float)_reflectionHeight/100.0f)); glVertex3f ( (destinationWidth/2.0f), (destinationHeight*((float)_reflectionHeight/100.0f))-1.0f, 0);
				glTexCoord2f (0.0f, 1.0f-((float)_reflectionHeight/100.0f)); glVertex3f (-(destinationWidth/2.0f), (destinationHeight*((float)_reflectionHeight/100.0f))-1.0f, 0);
			glEnd ();
			glDisable (GL_TEXTURE_2D);

			// reflet degrade de la pochette
			glBegin (GL_QUADS);
				glColor4f  (_tableColour.getRed()/255, _tableColour.getGreen()/255, _tableColour.getBlue()/255, alpha/2);
				glVertex3f (-(destinationWidth/2.0f), -1.0f, 0);
				glVertex3f ( (destinationWidth/2.0f), -1.0f, 0);
				glColor4f  (_tableColour.getRed()/255, _tableColour.getGreen()/255, _tableColour.getBlue()/255, 1);
				glVertex3f ( (destinationWidth/2.0f), (destinationHeight*((float)_reflectionHeight/100.0f))-1.0f, 0);
				glVertex3f (-(destinationWidth/2.0f), (destinationHeight*((float)_reflectionHeight/100.0f))-1.0f, 0);
			glEnd ();
		}
	}

	glDisable (GL_DEPTH_TEST);
	glDisable (GL_BLEND);

	glPopMatrix ();
}
//=============================================================================
GLuint CoverFlowOpenGL::loadTexture (Image textureImg)
{
	/* To get texture ID */
	GLuint textureTemp;

	glPixelStorei (GL_UNPACK_ALIGNMENT, 4);

	const void* pixels;

#if JUCE_GIT
	Image::BitmapData datas (textureImg, 0, 0, textureImg.getWidth(), textureImg.getHeight());
	pixels = datas.getPixelPointer (0, 0);
#else
	int stride, pixStride;
	pixels = textureImg->lockPixelDataReadOnly (0, 0, textureImg->getWidth(), textureImg->getHeight(), stride, pixStride);
#endif

	/* Generate texture */
	glGenTextures (1, &textureTemp);
	glBindTexture (GL_TEXTURE_2D, textureTemp);

	/* Setup texture filters */
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D  (GL_TEXTURE_2D, 
				   0, 4, textureImg.getWidth(), textureImg.getHeight(), 
				   0, GL_BGR_EXT, GL_UNSIGNED_BYTE, 
				   pixels);

#if JUCE_GIT
	// do nothing
#else
	textureImg->releasePixelDataReadOnly (pixels);
#endif

	return textureTemp;
}
//=============================================================================
GLuint CoverFlowOpenGL::loadTextureAlpha (Image textureImg)
{
	/* To get texture ID */
	GLuint textureTemp;

	const void* pixels;

#if JUCE_GIT
	Image::BitmapData datas (textureImg, 0, 0, textureImg.getWidth(), textureImg.getHeight());
	pixels = datas.getPixelPointer (0, 0);
#else
	int stride, pixStride;
	pixels = textureImg->lockPixelDataReadOnly (0, 0, textureImg->getWidth(), textureImg->getHeight(), stride, pixStride);
#endif
	
	/* Generate texture */
	glGenTextures (1, &textureTemp);
	glBindTexture (GL_TEXTURE_2D, textureTemp);

	/* Setup texture filters */
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D  (GL_TEXTURE_2D, 
				   0, 4, textureImg.getWidth(), textureImg.getHeight(),
				   0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, 
				   pixels);

#if JUCE_GIT
	// do nothing
#else
	textureImg->releasePixelDataReadOnly (pixels);
#endif

	return textureTemp;
}
//=============================================================================
void CoverFlowOpenGL::removeTexture (GLuint textureID)
{
	int toRemove = -1;
	for (int i = 0; i < _itemsTextures.size(); i++) 
	{
		if (_itemsTextures[i]->ID == textureID)
		{
			glDeleteTextures (1, (const GLuint *)_itemsTextures[i]->ID);
			toRemove = i;
			break;
		}
	}	
	if (toRemove != -1)
		_itemsTextures.remove (toRemove);
}
//=============================================================================