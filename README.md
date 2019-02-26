# Subdivision Surface Modeler

__Summary:__
 
This program displays a mesh object that can manipulated in various ways.  The main 
function of this program is to use Catmull-Clark subdivision to smooth out a mesh 
consisting of quadrilateral faces.  The mesh is stored in a pseudo Winged-Edge 
representation where each face, edge, and vertex points to the faces, edges, and 
vertices that it consists of or belongs to.  Users may generate a random heightfield 
using the input fields in the top right or select a model from the dropdown menu.  
Individual vertices may be selected and manipulated with the input fields in the middle 
right of the window.  

__Requirements :__

The program uses FLTK as its graphics library

__Controls:__

- W,A,S,D - Manipulates camera
- E,F - Zoom in and out
- Mouse - Drag to manipulate camera
- Mouse Wheel - Zoom in and out
