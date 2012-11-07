//=============================================================================
#include "CoverFlowComponent.h"
#include "CoverFlowContext.h"
//==============================================================================
CoverFlowComponent::CoverFlowComponent ()
	: background (0), 
	  firstCall (true)
{
	addAndMakeVisible (scrollbar = new ScrollBar());
	//scrollbar->setButtonVisibility (false);
	scrollbar->addListener (this);

	addAndMakeVisible (context = new CoverFlowContext ());
	context->startThread (5);
	//context->addComponentListener (this);
	context->addChangeListener (this);

	addAndMakeVisible (combobox = new ComboBox ("Plugins"));
	StringArray list = context->plugin.getPluginList ();
	int i=0; int max=list.size();
	for (; i<max; ++i)
		combobox->addItem (list[i], i+1);
	combobox->setSelectedId (1);
	combobox->addListener (this);
}
//==============================================================================
CoverFlowComponent::~CoverFlowComponent ()
{
	context->removeChangeListener (this);
	//context->removeComponentListener (this);

	deleteAllChildren ();
}
//==============================================================================
void CoverFlowComponent::paint (Graphics &g)
{
	Image back (Image::RGB, getWidth(), getHeight(), false);
	Graphics g1 (back);

#ifdef JUCE_GIT
	FillType gbrush (ColourGradient(Colours::black, 0.0f, getHeight()*0.15f,
									Colour(67,91,103), 0.0f, (float)getHeight(),
									false));
	g1.setFillType (gbrush);
	g1.fillAll ();

	FillType ibrush (ImageCache::getFromMemory (binary::grid_png, binary::grid_pngSize), 
					  AffineTransform::identity);
	ibrush.setOpacity (0.25f);

	g1.setFillType (ibrush);
	g1.fillAll ();
#else
	GradientBrush gbrush (Colours::black, 0.0f, getHeight()*0.15f,
						  Colour(67,91,103), 0.0f, (float)getHeight(),
						  false);

	g1.setBrush (&gbrush);
	g1.fillAll ();

	Image *brush = ImageCache::getFromMemory (binary::grid_png, binary::grid_pngSize);
	ImageBrush ibrush (brush, 0, 0, 1.0f);
	ibrush.multiplyOpacity (0.25f);

	g1.setBrush (&ibrush);
	g1.fillAll ();
#endif

	Rectangle r = context->getBounds ();

	background = new Image (Image::RGB, r.getWidth(), r.getHeight(), false);
	if (background)
	{
		Graphics g2 (*background);

		g2.drawImage (&back,
					  0, 0, r.getWidth(), r.getHeight(),
					  r.getX(), r.getY(), r.getWidth(), r.getHeight(),
					  false);

		g.drawImageAt (&back, 0, 0, false);

		context->addBackground (background);
		deleteAndZero (background);
	}
}
//==============================================================================
void CoverFlowComponent::resized ()
{
	const int top = 0;
	const int bottom = 18;
	const int bottomPadding = 12;
	const int padding = 36;

	context->setBounds (0, top, getWidth(), getHeight()-bottom-bottomPadding);
	scrollbar->setBounds ((padding*4), getHeight()-top-bottom-bottomPadding, 
						   getWidth()-(padding*8), bottom);
	combobox->setBounds ((padding*.5), getHeight()-top-bottom-bottomPadding, (padding*3), bottom);
}
//=============================================================================
void CoverFlowComponent::buttonClicked (Button *button)
{
}
//=============================================================================
void CoverFlowComponent::comboBoxChanged (ComboBox *combo)
{
	context->plugin.setPlugin (combo->getSelectedId());
}
//=============================================================================
void CoverFlowComponent::scrollBarMoved (ScrollBar *scrollBarThatHasMoved, 
									const double currentRangeStart)
{
	context->setCurrentSelected (roundDoubleToInt(currentRangeStart));
	delayedUpdate();
}
//=============================================================================
void CoverFlowComponent::changeListenerCallback (ChangeBroadcaster *objectThatHasChanged)
{
	if (objectThatHasChanged == context)
	{
		scrollbar->setCurrentRangeStart (context->getSelected());
		delayedUpdate();
	}
}
//=============================================================================
void CoverFlowComponent::componentMovedOrResized (Component &component, bool wasMoved, bool wasResized)
{
	/* auto-called right after creation */
	if (component.getName() == "CoverFlow" && wasResized)
	{
		/* Just delayed the first call a little
		   in order to be sure that OpenGL is initialized.
		   Because OpenGL must be initialized before the
		   createTemporaryCoverTextures() method is called.
		   Don't find another method than a "firstCall" to
		   create the OpenGL textures "on the fly".
		*/
		if (firstCall)
			startTimer (250);
		else
			startTimer (25);
	}
}
//=============================================================================
void CoverFlowComponent::timerCallback ()
{
	stopTimer ();
	if (firstCall)
	{
		firstCall = false;
		createTemporaryCoverTextures ();
	}
	delayedUpdate ();
}
//=============================================================================
void CoverFlowComponent::delayedUpdate ()
{
	/*
		Foreground
	*/
	Rectangle r = context->getBounds ();

	const int width = r.getWidth();
	const int height = r.getHeight();

	Image foreground (Image::ARGB, width, height, true);
	Graphics g1 (foreground);

	Font font;
	font.setBold (true);

	int top = 24;

	Image *stars = ImageCache::getFromMemory (binary::star80_png, binary::star80_pngSize);

	CoverFlowContext::CoverFlowItem *item = context->getItem (context->getSelected());

	g1.drawImage (item->img,
				  (stars->getWidth()*0.2f)/2, top, 
				   stars->getWidth()*0.8f, stars->getWidth()*0.8f,
				  0, 0, 128, 128, false);

	const int w = stars->getWidth();
	const int h = stars->getHeight();

	g1.drawImage (stars, 
				  0, top+(w*0.8f)-(h/4), w, h,
				  0, 0, w, h), false;

	top = 27;

	font.setHeight (28.0f);
	drawTextWithBorder (g1, &font,
						item->title.toUpperCase(),Justification::centredLeft, 
						Rectangle(w+(h/3), top, width-w, 28.f), 
						Colours::white, Colours::black, 5.f);

	font.setHeight (22.0f);
	drawTextWithBorder (g1, &font,
						item->artist.toUpperCase(), Justification::centredLeft, 
						Rectangle(w+(h/3), top+28.f-3.f, width, 22.f), 
						Colours::lightgrey, Colours::black, 4.f);

	font.setHeight (18.f);
	drawTextWithBorder (g1, &font,
						String("All Rights Reserved")/*.toUpperCase()*/, Justification::centredLeft, 
						Rectangle(w+(h/3), top+52.f-6.f, width, 18.f), 
						Colours::lightgrey.darker(0.25f), Colours::black, 3.f);

	context->addForeground (&foreground);

	// flush and repaint the GL Scene
	context->repaint ();
}
//==============================================================================
void CoverFlowComponent::createTemporaryCoverTextures ()
{
	String currentPath = File::getSpecialLocation(File::currentApplicationFile).getParentDirectory().getFullPathName();
	File xmlFile (currentPath + "\\coverflow.xml");
	XmlDocument doc (xmlFile);
	XmlElement *root = doc.getDocumentElement ();
	int total = 0;
	if (root)
	{
		String directory = root->getStringAttribute ("path");
		forEachXmlChildElementWithTagName (*root, child, "cover")
		{
			String image = child->getStringAttribute ("image");
			String filepath = currentPath + "\\" + directory + "\\" + image;
			Image* img = Image::loadFrom (File(filepath));
			if (img)
			{
				Graphics g (*img);
				g.setColour (Colours::black.brighter (0.1f));
				g.drawRect (0.0f, 0.0f, (float)img->getWidth(), (float)img->getHeight(), 1.0f);
				context->addItem (child->getStringAttribute ("artist"), 
								  child->getStringAttribute ("title"), 
								  img->createCopy());
				context->repaint ();
				delete img;
				total++;
			}
		}
		delete root;
	}
	double smin = 0.0;
	double smax = getWidth();
	double sstep = smax / total;
	scrollbar->setRangeLimits (smin, total);
	scrollbar->setCurrentRange (smin, 1.0); 
	scrollbar->setSingleStepSize (sstep/*1.0*/); 
}
//==============================================================================