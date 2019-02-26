#ifndef DISPLAY_H
#define DISPLAY_H

#include "viewPort.h"
#include "mesh.h"

// FLTK
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Color_Chooser.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Simple_Counter.H>
#include <Fl/Fl_Window.H>

class display : public Fl_Window		
{
private:
	mesh* m;
	viewPort* view;

	// Widgets
	Fl_Input* rowsInput;
	Fl_Input* columnsInput;
	Fl_Input* widthInput;
	Fl_Input* depthInput;
	Fl_Button* generateButton;
	Fl_Color_Chooser* colorChooser;
	Fl_Menu_Button* menuWidget;
	Fl_Button* catMullButton;
	Fl_Check_Button* wireButton;
	Fl_Check_Button* fillButton;
	Fl_Check_Button* highLightButton;
	Fl_Input* vertexInput;
	Fl_Button* vertexButton;
	Fl_Simple_Counter* vertexCounter;
	Fl_Input* xInput;
	Fl_Input* yInput;
	Fl_Input* zInput;
	Fl_Button* translateButton;
	Fl_Button* cameraButton;
	Fl_Button* aboveButton;
	Fl_Button* sideButton;

public:
	display(int x, int y, int w, int h, char* c);	// Constructor
	~display();										// Destructor

	// Methods //

	// Getters
	std::vector<face*> getFaces();

	// Callbacks
	static void generateCB(Fl_Widget *w, void *data);		// Generates a heightfield mesh from given parameters
	static void colorCB(Fl_Widget *w, void *data);			// Chooses color of mesh
	static void menuBoxCB(Fl_Widget *w, void *data);		// Generates a box mesh
	static void menuPyramidCB(Fl_Widget *w, void *data);	// Generates a pyramid mesh
	static void menuTCB(Fl_Widget *w, void *data);			// Generatex a T Shape mesh
	static void catMullCB(Fl_Widget *w, void *data);		// Calls the catMull function on the mesh
	static void wireCB(Fl_Widget *w, void *data);			// Toggle to show wireframe of mesh
	static void fillCB(Fl_Widget *w, void *data);			// Toggle to fill in mesh with color
	static void highLightCB(Fl_Widget *w, void *data);		// Toggle to highlight current selected vertex
	static void vertexButtonCB(Fl_Widget *w, void *data);	// Select a vertex in the mesh
	static void vertexCounterCB(Fl_Widget *w, void *data);	// Select next/prev vertex in the mesh
	static void translateCB(Fl_Widget *w, void *data);		// Translates a vertex by given values
	static void cameraCB(Fl_Widget *w, void *data);			// Set position of camera
	static void aboveCB(Fl_Widget *w, void *data);			// Rotate camera to an above view
	static void sideCB(Fl_Widget *w, void *data);			// Rotate camera to a side view
};

#endif