# Subdivision Surface Modeler

__Summary :__

In this program, a user can display, manipulate, and refine polygonal meshes.  The primary function demonstrated in this program is a smoothing algorithm based on the Catmull-Clark subdivision technique as it applies to quadrilateral faces.  Meshes are stored in a pseudo winged edge representation where each face, edge, and vertex points to the faces, edges, and vertices that it constitutes.  Users may generate a random heightfield or choose prefabricated polyhedral objects from the dropdown menu.  Individual vertices can be further manipulated using the available controls. 

__Libraries & Technologies Used :__
* FLTK - Fast Light Toolkit is a cross-platform C++ GUI toolkit.

__Controls :__
* W,A,S,D - Manipulates camera
* E,F - Zoom in and out
* Mouse - Drag to manipulate camera
* Mouse Wheel - Zoom in and out
