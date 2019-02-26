#ifndef VIEW_PORT_H
#define VIEW_PORT_H

#include "camera.h"
#include "algebra3.h"
#include "mesh.h"

// FLTK
#include <FL/Fl.H>
#include <FL/Gl.H>
#include <Fl/Fl_Gl_Window.H>
#include <GL/glu.h>

class viewPort : public Fl_Gl_Window
{
private:
	camera* cam;					
	mesh* m;

	int mouseX;							// Mouse position x and y
	int mouseY;							
	int meshWidth = 1;					// Distance between vertices in mesh is multiplied by meshWidth
	bool wireFrame = true;				// Show wireframe if true
	bool fillSurface = true;			// Draw mesh surface with fillColor if true
	bool showCurrentVertex = false;		// Highlight currently selected vertex if true
	vec3 fillColor = vec3(0, .5, 0);	// Dark green color

	// Methods //
	int handle(int event);			// Handle mouse and keyboard events
	void init();					// Initialize the required OpenGL transforms
	void draw();					// Calls draw on the Graphics Window

public:
	viewPort(int x, int y, int w, int h, mesh* m);	// Constructor
	~viewPort();									// Destructor

	// Methods //
	void toggleWire();
	void toggleFill();
	void toggleHighLight();

	// Getters
	camera* getCam();

	// Setters
	void setFillColor(vec3 color);
	void setMeshWidth(int i);
	void setMesh(mesh* newMesh);
};

#endif