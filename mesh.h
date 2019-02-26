#ifndef MESH_H
#define MESH_H 

#include <vector>
#include "algebra3.h"

class mesh;  // Forward declarations
class face;
class edge;

class vertex
{
public:
	vertex(vec4 v);

	vec4 position;
	std::vector<face*> faces;
	std::vector<edge*> edges;
};

class edge
{
public:
	edge(vertex* a, vertex* b);

	bool added = false;		// Used to make sure an edge is only added to mesh->edges[] once
	vertex* edgepoint;		// Vertex created from the average of its two original end points and two adjacent facepoints
	vertex* v1;
	vertex* v2;
	std::vector<face*> faces;
};

class face
{
public:
	face(vertex* a, vertex* b, vertex* c, vertex* d, mesh* m);

	bool edgeFound = false;		// Used in finding if an edge already exists between two vertices in a face
	vertex* facepoint;			// Vertex created from the average of the four original vertices in a face
	mesh* parentMesh;
	std::vector<edge*> edges;
	std::vector<vertex*> vertices;
};

class mesh
{
private:
	vertex* currentVertex;		// Vertex that is currently selected for translation

public:
	mesh(int r, int c, int w, int d);		// Constructor for random height field
	mesh(int i);							// Constructor for the model meshes
	~mesh();								// Destructor

	std::vector<vertex*> vertices;
	std::vector<edge*> edges;
	std::vector<face*> faces;

	// Methods //
	void catMull(int iterations);					// Catmull-Clark subdivision - Subdivides and smoothes a mesh
	edge* findCommonEdge(vertex* v1, vertex* v2);	// Returns the common edge between two vertices

	// Getters
	vec4 getCurrentVertexPos();

	// Setters
	void setCurrentVertexPos(vec4 v);
	void setCurrentVertex(vertex* v);

	void generateHeightField(int r, int c, int w, int d);	// Generates a random heightfield mesh
	void generateCube();									// Generates a box mesh
	void generatePyramid();									// Generates a pyramid mesh
	void generateT();										// Generates a T shape mesh
};

#endif