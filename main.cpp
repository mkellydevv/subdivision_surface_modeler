#include "display.h"

int main(int argc, char * const argv[]) {
	display* disp = new display(300, 0, 950, 700, "Subdivision Surface Modeler");		// Create a FL Window
	disp->show();																		
	return Fl::run();																	
}