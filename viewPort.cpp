#include "viewPort.h"
#include "display.h"

viewPort::viewPort(int x, int y, int w, int h, mesh* m) : Fl_Gl_Window(x, y, w, h)
{
	cam = new camera(8, 0, 0);
	this->m = m;
}

viewPort::~viewPort()
{
	delete cam;
}

int
viewPort::handle(int event){
	// When a key is released, check for an event and handle it
	if (event == FL_KEYUP){
		// Rotate and zoom camera with w,a,s,d and e,f
		if (Fl::event_key() == 'a'){
			cam->increasePhi(-10);
			redraw();
		}
		if (Fl::event_key() == 'd'){
			cam->increasePhi(10);
			redraw();
		}
		if (Fl::event_key() == 'w'){
			cam->increaseTheta(-10);
			redraw();
		}
		if (Fl::event_key() == 's'){
			cam->increaseTheta(10);
			redraw();
		}
		if (Fl::event_key() == 'e'){
			cam->increaseRad(-.5);
			redraw();
		}
		if (Fl::event_key() == 'f'){
			cam->increaseRad(.5);
			redraw();
		}
	}
	// When left mouse button is held down, prepare for drag event
	if (event == FL_PUSH && Fl::event_button() == FL_LEFT_MOUSE)
	{
		mouseX = Fl::event_x();
		mouseY = Fl::event_y();
		return 1;
	}
	// With left mouse button held down, drag to rotate camera
	if (event == FL_DRAG)
	{
		if (mouseX < Fl::event_x())
			cam->increasePhi(-2);
		if (mouseX > Fl::event_x())
			cam->increasePhi(2);

		if (mouseY < Fl::event_y())
			cam->increaseTheta(-2);
		if (mouseY > Fl::event_y())
			cam->increaseTheta(2);

		redraw();

		mouseX = Fl::event_x();
		mouseY = Fl::event_y();
	}
	// Rotate mouse wheel to zoom in and out
	if (event == FL_MOUSEWHEEL)
	{
		if (Fl::event_dy() < 0)
			cam->increaseRad(-.5);
		else
			cam->increaseRad(.5);
		redraw();
	}
	return Fl_Gl_Window::handle(event);
}

void
viewPort::init(){		// Initialize the required OpenGL transforms
	glViewport(0, 0, w(), h());
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, 1, .1, 50);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);
}

void
viewPort::draw()
{
	if (!valid()){
		init();		// Make sure the OpenGL settings are completed once before drawing
		valid(1);
	}

	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gluLookAt(cam->getEye()[0], cam->getEye()[1], cam->getEye()[2],		// Set the camera
		cam->getCenter()[0] * meshWidth, cam->getCenter()[1] * meshWidth, cam->getCenter()[2] * meshWidth,
		cam->getUp()[0], cam->getUp()[1], cam->getUp()[2]);


	Fl_Group* g = this->parent();		// Get a pointer to the FL display window
	display* d = (display*)(g);
	std::vector<face*> faces = d->getFaces();

	// Start drawing here
	for (int i = 0; i < faces.size(); i++)
	{
		if (fillSurface)
		{
			glColor3f(fillColor[0], fillColor[1], fillColor[2]);
			glBegin(GL_QUADS);
			glVertex3f(faces[i]->vertices[0]->position[0] * meshWidth, faces[i]->vertices[0]->position[1] * meshWidth, faces[i]->vertices[0]->position[2] * meshWidth);
			glVertex3f(faces[i]->vertices[1]->position[0] * meshWidth, faces[i]->vertices[1]->position[1] * meshWidth, faces[i]->vertices[1]->position[2] * meshWidth);
			glVertex3f(faces[i]->vertices[2]->position[0] * meshWidth, faces[i]->vertices[2]->position[1] * meshWidth, faces[i]->vertices[2]->position[2] * meshWidth);
			glVertex3f(faces[i]->vertices[3]->position[0] * meshWidth, faces[i]->vertices[3]->position[1] * meshWidth, faces[i]->vertices[3]->position[2] * meshWidth);
			glEnd();
		}

		if (wireFrame)
		{
			glColor3f(1, 1, 1);
			glBegin(GL_LINE_LOOP);
			glVertex3f(faces[i]->vertices[0]->position[0] * meshWidth, faces[i]->vertices[0]->position[1] * meshWidth, faces[i]->vertices[0]->position[2] * meshWidth);
			glVertex3f(faces[i]->vertices[1]->position[0] * meshWidth, faces[i]->vertices[1]->position[1] * meshWidth, faces[i]->vertices[1]->position[2] * meshWidth);
			glVertex3f(faces[i]->vertices[2]->position[0] * meshWidth, faces[i]->vertices[2]->position[1] * meshWidth, faces[i]->vertices[2]->position[2] * meshWidth);
			glVertex3f(faces[i]->vertices[3]->position[0] * meshWidth, faces[i]->vertices[3]->position[1] * meshWidth, faces[i]->vertices[3]->position[2] * meshWidth);
			glEnd();
		}

		// Draw a yellow box around the currently selected vertex
		if (showCurrentVertex)
		{
			glColor3f(1, 1, 0);
			vec4 pos = m->getCurrentVertexPos();
			glBegin(GL_QUADS);
			glVertex3f(pos[0] * meshWidth - .1, pos[1] * meshWidth - .1, pos[2] * meshWidth);
			glVertex3f(pos[0] * meshWidth - .1, pos[1] * meshWidth + .1, pos[2] * meshWidth);
			glVertex3f(pos[0] * meshWidth + .1, pos[1] * meshWidth + .1, pos[2] * meshWidth);
			glVertex3f(pos[0] * meshWidth + .1, pos[1] * meshWidth - .1, pos[2] * meshWidth);
			glEnd();
		}
	}
}

void
viewPort::toggleWire()
{
	wireFrame = !wireFrame;
}

void
viewPort::toggleFill()
{
	fillSurface = !fillSurface;
}

void
viewPort::toggleHighLight()
{
	showCurrentVertex = !showCurrentVertex;
}

camera*
viewPort::getCam()
{
	return cam;
}

void
viewPort::setFillColor(vec3 color)
{
	fillColor = color;
}

void
viewPort::setMeshWidth(int i)
{
	meshWidth = i;
}

void
viewPort::setMesh(mesh* newMesh)
{
	m = newMesh;
}