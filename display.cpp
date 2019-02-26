#include "display.h"

display::display(int x, int y, int w, int h, char* c) : Fl_Window(x, y, w, h, c)
{
	m = new mesh(4, 4, 1, 4);
	view = new viewPort(0, 0, 700, 700, m);

	int offSet = 700;	// Offset same as view ports width

	rowsInput = new Fl_Input(55 + offSet, 0, 40, 30, "Rows:");
	columnsInput = new Fl_Input(55 + offSet, 30, 40, 30, "Cols:");
	widthInput = new Fl_Input(55 + offSet, 60, 40, 30, "Width:");
	depthInput = new Fl_Input(55 + offSet, 90, 40, 30, "Depth:");
	generateButton = new Fl_Button(offSet, 120, 100, 30, "New Heightmap");
	generateButton->callback(this->generateCB, this);

	colorChooser = new Fl_Color_Chooser(100 + offSet, 0, 150, 150);
	colorChooser->rgb(0, 0.5, 0);
	colorChooser->changed();
	colorChooser->callback(this->colorCB, this);

	menuWidget = new Fl_Menu_Button(offSet, 155, 100, 30, "Models");
	menuWidget->add("Box", "", menuBoxCB, this, 0);
	menuWidget->add("Pyramid", "", menuPyramidCB, this, 0);
	menuWidget->add("T Shape", "", menuTCB, this, 0);

	catMullButton = new Fl_Button(125 + offSet, 155, 100, 90, "Refine Mesh");
	catMullButton->callback(this->catMullCB, this);

	wireButton = new Fl_Check_Button(offSet, 250, 30, 30, "Wire Frame");
	wireButton->value(1);
	wireButton->callback(this->wireCB, this);

	fillButton = new Fl_Check_Button(100 + offSet, 250, 30, 30, "Fill");
	fillButton->value(1);
	fillButton->callback(this->fillCB, this);

	highLightButton = new Fl_Check_Button(145 + offSet, 250, 30, 30, "Show Vertex");
	highLightButton->value(0);
	highLightButton->callback(this->highLightCB, this);

	vertexInput = new Fl_Input(55 + offSet, 285, 40, 30, "Vertex:");
	vertexButton = new Fl_Button(offSet, 320, 100, 30, "Find Vertex");
	vertexButton->callback(this->vertexButtonCB, this);
	vertexCounter = new Fl_Simple_Counter(120 + offSet, 285, 100, 30, "Vertex Chooser");
	vertexCounter->step(1);
	vertexCounter->bounds(0, m->vertices.size() - 1);
	vertexCounter->callback(this->vertexCounterCB, this);

	xInput = new Fl_Input(55 + offSet, 365, 30, 30, "X:");
	xInput->value("0");
	yInput = new Fl_Input(55 + offSet, 395, 30, 30, "Y:");
	yInput->value("0");
	zInput = new Fl_Input(55 + offSet, 425, 30, 30, "Z:");
	zInput->value("0");
	translateButton = new Fl_Button(115 + offSet, 375, 120, 30, "Translate Vertex");
	translateButton->callback(this->translateCB, this);

	cameraButton = new Fl_Button(105 + offSet, 415, 140, 30, "Set Camera Position");
	cameraButton->callback(this->cameraCB, this);

	aboveButton = new Fl_Button(offSet, 460, 125, 30, "Top View");
	aboveButton->callback(this->aboveCB, this);

	sideButton = new Fl_Button(125 + offSet, 460, 125, 30, "Side View");
	sideButton->callback(this->sideCB, this);
}

display::~display()
{
	delete m;
	delete view;
	delete rowsInput;
	delete columnsInput;
	delete widthInput;
	delete depthInput;
	delete generateButton;
	delete colorChooser;
	delete menuWidget;
	delete catMullButton;
	delete wireButton;
	delete fillButton;
	delete highLightButton;
	delete vertexInput;
	delete vertexButton;
	delete vertexCounter;
	delete xInput;
	delete yInput;
	delete zInput;
	delete translateButton;
	delete cameraButton;
	delete aboveButton;
	delete sideButton;
}

std::vector<face*>
display::getFaces()
{
	return m->faces;
}

// Callbacks //

void
display::generateCB(Fl_Widget *w, void *data)
{
	display* disp = (display*)data;
	
	int r = atoi(disp->rowsInput->value());
	int c = atoi(disp->columnsInput->value());
	int wi = atoi(disp->widthInput->value());
	int d = atoi(disp->depthInput->value());

	if (r == 0 || c == 0)
	{
		printf("The number of rows and columns must be integers greater than zero.");
	}
	else if (wi == 0)
	{
		printf("Width must be an integer greater than zero.");
	}
	else if (d == 0)
	{
		printf("Depth must be an integer greater than zero.");
	}
	else
	{
		delete disp->m;
		disp->m = new mesh(r, c, wi, d);
		disp->view->setMesh(disp->m);
		disp->view->setMeshWidth(wi);
		disp->vertexCounter->value(0);
		disp->vertexCounter->bounds(0, disp->m->vertices.size() - 1);
		disp->view->redraw();
	}
}

void
display::colorCB(Fl_Widget *w, void *data)
{
	display* disp = (display*)data;
	vec3 col = vec3(disp->colorChooser->r(), disp->colorChooser->g(), disp->colorChooser->b());
	disp->view->setFillColor(col);
	disp->colorChooser->redraw();
	disp->view->redraw();
}

void
display::menuBoxCB(Fl_Widget *w, void *data)
{
	display* disp = (display*)data;
	delete disp->m;
	disp->m = new mesh(0);
	disp->view->setMesh(disp->m);
	disp->view->setMeshWidth(1);
	disp->vertexCounter->value(0);
	disp->vertexCounter->bounds(0, disp->m->vertices.size() - 1);
	disp->view->redraw();
}

void
display::menuPyramidCB(Fl_Widget *w, void *data)
{
	display* disp = (display*)data;
	delete disp->m;
	disp->m = new mesh(1);
	disp->view->setMesh(disp->m);
	disp->view->setMeshWidth(1);
	disp->vertexCounter->value(0);
	disp->vertexCounter->bounds(0, disp->m->vertices.size() - 1);
	disp->view->redraw();
}

void
display::menuTCB(Fl_Widget *w, void *data)
{
	display* disp = (display*)data;
	delete disp->m;
	disp->m = new mesh(2);
	disp->view->setMesh(disp->m);
	disp->view->setMeshWidth(1);
	disp->vertexCounter->value(0);
	disp->vertexCounter->bounds(0, disp->m->vertices.size() - 1);
	disp->view->redraw();
}

void
display::catMullCB(Fl_Widget *w, void *data)
{
	display* disp = (display*)data;
	disp->m->catMull(1);
	disp->m->setCurrentVertex(disp->m->vertices[0]);
	disp->vertexCounter->value(0);
	disp->vertexCounter->bounds(0, disp->m->vertices.size() - 1);
	disp->view->redraw();
}

void
display::wireCB(Fl_Widget *w, void *data)
{
	display* disp = (display*)data;
	disp->view->toggleWire();
	disp->view->redraw();
}

void
display::fillCB(Fl_Widget *w, void *data)
{
	display* disp = (display*)data;
	disp->view->toggleFill();
	disp->view->redraw();
}

void
display::highLightCB(Fl_Widget *w, void *data)
{
	display* disp = (display*)data;
	disp->view->toggleHighLight();
	disp->view->redraw();
}

void
display::vertexButtonCB(Fl_Widget *w, void *data)
{
	display* disp = (display*)data;
	int val = atoi(disp->vertexInput->value());
	int tmp = disp->m->vertices.size() - 1;
	if (val > tmp)									// If vertex value is out of range, set it to first or last vertex in the vector
	{
		val = tmp;
	}
	else if (val < 0)
	{
		val = 0;
	}
	disp->vertexInput->value("");
	disp->vertexCounter->value(val);
	disp->m->setCurrentVertex(disp->m->vertices[val]);
	disp->view->redraw();
}

void
display::vertexCounterCB(Fl_Widget *w, void *data)
{
	display* disp = (display*)data;
	int i = disp->vertexCounter->value();
	disp->m->setCurrentVertex(disp->m->vertices[i]);
	disp->view->redraw();
}

void
display::translateCB(Fl_Widget *w, void *data)
{
	display* disp = (display*)data;
	vec4 newPos = disp->m->getCurrentVertexPos();
	int xVal = atoi(disp->xInput->value());
	int yVal = atoi(disp->yInput->value());
	int zVal = atoi(disp->zInput->value());
	newPos[0] += xVal;
	newPos[1] += yVal;
	newPos[2] += zVal;
	disp->m->setCurrentVertexPos(newPos);
	disp->xInput->value("0");
	disp->yInput->value("0");
	disp->zInput->value("0");
	disp->view->redraw();
}

void
display::cameraCB(Fl_Widget *w, void *data)
{
	display* disp = (display*)data;
	double xVal = atof(disp->xInput->value());
	double yVal = atof(disp->yInput->value());
	double zVal = atof(disp->zInput->value());
	disp->view->getCam()->setCenter(vec4(xVal, yVal, zVal, 0));
	disp->xInput->value("0");
	disp->yInput->value("0");
	disp->zInput->value("0");
	disp->view->redraw();
}

void
display::aboveCB(Fl_Widget *w, void *data)
{
	display* disp = (display*)data;
	disp->view->getCam()->setTopView();
	disp->view->redraw();
}

void
display::sideCB(Fl_Widget *w, void *data)
{
	display* disp = (display*)data;
	disp->view->getCam()->setSideView();
	disp->view->redraw();
}