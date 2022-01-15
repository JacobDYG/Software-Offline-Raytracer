//////////////////////////////////////////////////////////////////////
//
//  University of Leeds
//  COMP 5812M Foundations of Modelling & Rendering
//  User Interface for Coursework
//
//  September, 2020
//
//  -----------------------------
//  Raytrace Render Widget
//  -----------------------------
//
//	Provides a widget that displays a fixed image
//	Assumes that the image will be edited (somehow) when Render() is called
//  
////////////////////////////////////////////////////////////////////////

// include the header file
#include "RaytraceRenderWidget.h"

// constructor
RaytraceRenderWidget::RaytraceRenderWidget
		(   
		// the geometric object to show
		TexturedObject      *newTexturedObject,
		// the render parameters to use
		RenderParameters    *newRenderParameters,
		// parent widget in visual hierarchy
		QWidget             *parent
		)
	// the : indicates variable instantiation rather than arbitrary code
	// it is considered good style to use it where possible
	: 
	// start by calling inherited constructor with parent widget's pointer
	QOpenGLWidget(parent),
	// then store the pointers that were passed in
	texturedObject(newTexturedObject),
	renderParameters(newRenderParameters),
	raytracer(&frameBuffer)
	{ // constructor
	// leaves nothing to put into the constructor body
	} // constructor    

// destructor
RaytraceRenderWidget::~RaytraceRenderWidget()
	{ // destructor
	// empty (for now)
	// all of our pointers are to data owned by another class
	// so we have no responsibility for destruction
	// and OpenGL cleanup is taken care of by Qt
	} // destructor                                                                 

void RaytraceRenderWidget::invokeRt()
{
	// Call raytrace
	Raytrace();
}

// called when OpenGL context is set up
void RaytraceRenderWidget::initializeGL()
	{ // RaytraceRenderWidget::initializeGL()
	// this should remain empty
	} // RaytraceRenderWidget::initializeGL()

// called every time the widget is resized
void RaytraceRenderWidget::resizeGL(int w, int h)
	{ // RaytraceRenderWidget::resizeGL()
	// resize the render image
	frameBuffer.Resize(w, h);
	} // RaytraceRenderWidget::resizeGL()
	
// called every time the widget needs painting
void RaytraceRenderWidget::paintGL()
	{ // RaytraceRenderWidget::paintGL()
	// set background colour to white
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	// and display the image
	glDrawPixels(frameBuffer.width, frameBuffer.height, GL_RGBA, GL_UNSIGNED_BYTE, frameBuffer.block);
	} // RaytraceRenderWidget::paintGL()
	
	// routine that generates the image
void RaytraceRenderWidget::Raytrace()
{ // RaytraceRenderWidget::Raytrace()
	// Write gradient to image as test
	for (size_t col = 0; col < frameBuffer.width; col++)
	{
		for (size_t row = 0; row < frameBuffer.height; row++)
		{
			auto r = double(col) / (frameBuffer.width - 1);
			auto g = double(row) / (frameBuffer.height - 1);
			auto b = 0.25f;

			int rInt = static_cast<int>(255.999 * r);
			int gInt = static_cast<int>(255.999 * g);
			int bInt = static_cast<int>(255.999 * b);

			frameBuffer[row][col] = RGBAValue(rInt, gInt, bInt, 1.0f);
		}
	}
	//raytracer.setProjectionPerspective();
	raytracer.raytrace();
	
} // RaytraceRenderWidget::Raytrace()
	
// mouse-handling
void RaytraceRenderWidget::mousePressEvent(QMouseEvent *event)
	{ // RaytraceRenderWidget::mousePressEvent()
	// store the button for future reference
	int whichButton = event->button();
	// scale the event to the nominal unit sphere in the widget:
	// find the minimum of height & width   
	float size = (width() > height()) ? height() : width();
	// scale both coordinates from that
	float x = (2.0 * event->x() - size) / size;
	float y = (size - 2.0 * event->y() ) / size;

	
	// and we want to force mouse buttons to allow shift-click to be the same as right-click
	int modifiers = event->modifiers();
	
	// shift-click (any) counts as right click
	if (modifiers & Qt::ShiftModifier)
		whichButton = Qt::RightButton;
	
	// send signal to the controller for detailed processing
	emit BeginScaledDrag(whichButton, x,y);
	} // RaytraceRenderWidget::mousePressEvent()
	
void RaytraceRenderWidget::mouseMoveEvent(QMouseEvent *event)
	{ // RaytraceRenderWidget::mouseMoveEvent()
	// scale the event to the nominal unit sphere in the widget:
	// find the minimum of height & width   
	float size = (width() > height()) ? height() : width();
	// scale both coordinates from that
	float x = (2.0 * event->x() - size) / size;
	float y = (size - 2.0 * event->y() ) / size;
	
	// send signal to the controller for detailed processing
	emit ContinueScaledDrag(x,y);
	} // RaytraceRenderWidget::mouseMoveEvent()
	
void RaytraceRenderWidget::mouseReleaseEvent(QMouseEvent *event)
	{ // RaytraceRenderWidget::mouseReleaseEvent()
	// scale the event to the nominal unit sphere in the widget:
	// find the minimum of height & width   
	float size = (width() > height()) ? height() : width();
	// scale both coordinates from that
	float x = (2.0 * event->x() - size) / size;
	float y = (size - 2.0 * event->y() ) / size;
	
	// send signal to the controller for detailed processing
	emit EndScaledDrag(x,y);
	} // RaytraceRenderWidget::mouseReleaseEvent()
