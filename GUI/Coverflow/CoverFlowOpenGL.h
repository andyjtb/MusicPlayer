// =============================================================================
#ifndef __COVERFLOW_OPENGL_H__
#define __COVERFLOW_OPENGL_H__
// =============================================================================
#include "includes.h"
// =============================================================================
#ifdef _WIN32
 #include <gl/gl.h>
 #include <gl/glu.h>
#elif defined (LINUX)
 #include <GL/gl.h>
 #include <GL/glut.h>
 #undef KeyPress
#else
 #include <GLUT/glut.h>
#endif
// =============================================================================
#define GL_CLAMP_TO_EDGE      0x812F
#define GL_BGRA               0x80E1
// =============================================================================
#define BUFSIZE               4096
// =============================================================================
#include "CoverFlowPlugin.h"
// =============================================================================
class CoverFlowContext;
// =============================================================================
//struct OpenGLTexture
//{
//	GLuint ID;
//	int width;
//	int height;
//	bool isLoaded;
//
//	OpenGLTexture (GLuint i, const int w, const int h, const bool l)
//		: ID (i), width (w), height (h), isLoaded (l)
//	{}
//};
// =============================================================================
typedef OpenGLTexture Texture;
typedef OwnedArray <Texture> Textures;
// =============================================================================
class CoverFlowOpenGL
{
	public:
		CoverFlowOpenGL (CoverFlowContext* parent);
		~CoverFlowOpenGL ();
		// =====================================================================
		void initGL (const int newWidth, const int newHeight);
		void resizeGL (const int newWidth, const int newHeight);
		void pickingGL ();
		void repaintGL ();
		// =====================================================================
		GLuint loadTexture (Image& textureImg);
		GLuint loadTextureAlpha (Image* textureImg);
		void removeTexture (GLuint textureID);
		// =====================================================================
		void enable2D ();
		void disable2D ();
		// =====================================================================
		void drawBackground ();
		void drawForeground ();
		// =====================================================================
		void drawItems (int index, 
						bool reflet, 
						bool picking);
		// =====================================================================
		void drawItem (float textureWidth, 
					   float textureHeight,
					   bool reflection,
					   float alphaf,
					   int index);
		// =====================================================================
		void viewItems (bool reflection,
						bool picking);
		// =====================================================================
	protected:
		friend class CoverFlowContext;
		OpenGLTexture*	_backgroundTexture;
		OpenGLTexture*	_foregroundTexture;
		Textures	_itemsTextures;
		// =====================================================================
	private:
		// =====================================================================
		CoverFlowContext*	 parent;
		// =====================================================================
		int			_currentWidth;
		int			_currentHeight;
		// =====================================================================
		/* Skin */
		int			_animSelected;
		float		_sceneZoom;
		float		_verticalMove;
		float		_horizontaMove;
		int			_verticalAngle;
		int			_horizontalAngle;
		bool		_reflectionActive;
		int			_reflectionHeight;
		bool		_keepAspectRatio;
		// =====================================================================
		/* */
		Colour		_backColour;
		Colour		_tableColour;
		// =====================================================================
		friend class CoverFlowContext;
		friend class CoverFlowComponent;
		// =====================================================================
		CoverFlowPlugin		 plugin;
		// =====================================================================
		/* picking */
		int			_pickingX;
		int			_pickingY;
		GLint		_lastPicking;
		// =====================================================================
		struct PickingInfo {
			int   id; // Picking ID
			float z;  // Z-Order value
		};
		// =====================================================================
		OwnedArray <PickingInfo> listPicking;
		// Compute and render the Picking
		void selectPicking (int x, int y);
		// Organize the Picking informations
		void processHits (GLint hits, GLuint buffer[], OwnedArray<PickingInfo>& list);
		// Just a Z-Order sorting function for the array
		class ComparePickingInfo {
		public:
			ComparePickingInfo () {}
			int compareElements (PickingInfo* a, PickingInfo* b) {
				return b->z > a->z;
			}
		};
		ComparePickingInfo sorter;
		// =====================================================================
};
// =============================================================================
#endif // __COVERFLOW_OPENGL_H__
// =============================================================================