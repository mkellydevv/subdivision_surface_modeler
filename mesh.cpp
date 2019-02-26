#include "mesh.h"

vertex::vertex(vec4 vec)
{
	position = vec;
}

edge::edge(vertex* a, vertex* b)
{
	v1 = a;
	v2 = b;
}

face::face(vertex* a, vertex* b, vertex* c, vertex* d, mesh* m)
{
	// Adds the four vertices to this face
	vertices.push_back(a);
	vertices.push_back(b);
	vertices.push_back(c);
	vertices.push_back(d);
	parentMesh = m;

	// Adds this face to the four vertices
	for (int i = 0; i < vertices.size(); i++)
	{
		vertices[i]->faces.push_back(this);
	}

	// Edges are created or added to the faces here
	int y = 0;	// y is the next vertex after x
	for (int x = 0; x < vertices.size(); x++)
	{
		if (y == vertices.size() - 1)
		{
			y = -1;		// Sets y to 0 when x is 3
		}
		y++;
		// Looks for a common edge already existing between vertex x and vertex y
		for (int i = 0; i < vertices[x]->edges.size(); i++)
		{
			for (int j = 0; j < vertices[y]->edges.size(); j++)
			{
				if (vertices[x]->edges[i] == vertices[y]->edges[j])
				{
					edges.push_back(vertices[x]->edges[i]);			// Adds the found edge to this face
					vertices[x]->edges[i]->faces.push_back(this);	// Adds this face to the found edge
					edgeFound = true;
				}
			}
		}
		if (!edgeFound)		// Create new edge between x and y if one was not found to exist already
		{
			edge* e1 = new edge(vertices[x], vertices[y]);
			parentMesh->edges.push_back(e1);			// Add new edge to parent mesh
			e1->faces.push_back(this);					// Add this face to new edge
			edges.push_back(e1);						// Add new edge to this face
			vertices[x]->edges.push_back(e1);			// Add new edge to vertex x and y
			vertices[y]->edges.push_back(e1);
		}
		edgeFound = false;
	}
}

mesh::mesh(int r, int c, int w, int d)
{
	generateHeightField(r, c, w, d);
	currentVertex = vertices[0];

	mat4 trans = translation3D(vec3(-((double)r / 2), -((double)c / 2), 0));		// Translate the mesh for easier camera manipulation
	for (int j = 0; j < vertices.size(); j++)
	{
		vertices[j]->position = trans * vertices[j]->position;
		vertices[j]->position = vec4(vertices[j]->position[0] / vertices[j]->position[3], vertices[j]->position[1] / vertices[j]->position[3],
			vertices[j]->position[2] / vertices[j]->position[3], 1);
	}
}

mesh::mesh(int i)
{
	mat4 trans;
	if (i == 0)
	{
		generateCube();
		trans = translation3D(vec3(-.5, -.5, -.5));
	}
	else if (i == 1)
	{
		generatePyramid();
		trans = translation3D(vec3(-.5, -.5, -.5));
	}
	else
	{
		generateT();
		trans = translation3D(vec3(-.5, -1, -.5));
	}
	currentVertex = vertices[0];

	for (int j = 0; j < vertices.size(); j++)		// Translate the mesh for easier camera manipulation
	{
		vertices[j]->position = trans * vertices[j]->position;
		vertices[j]->position = vec4(vertices[j]->position[0] / vertices[j]->position[3], vertices[j]->position[1] / vertices[j]->position[3],
			vertices[j]->position[2] / vertices[j]->position[3], 1);
	}
}

mesh::~mesh() 
{
	for (int i = 0; i < vertices.size(); i++)
	{
		delete vertices[i];
	}
	for (int j = 0; j < edges.size(); j++)
	{
		delete edges[j];
	}
	for (int k = 0; k < faces.size(); k++)
	{
		delete faces[k];
	}
}

void
mesh::catMull(int iterations)
{
	for (int i = 0; i < iterations; i++)
	{
		// Find the facepoint for each face by averaging the original vertices in a face
		for (int i = 0; i < faces.size(); i++)
		{
			vec4 fp = vec4(0, 0, 0, 0);
			for (int j = 0; j < faces[i]->vertices.size(); j++)
			{
				fp = fp + faces[i]->vertices[j]->position;
			}
			fp = fp / faces[i]->vertices.size();
			faces[i]->facepoint = new vertex(fp);
		}
		// Find the edgepoints for each edge by averaging the two original end points and two adjacent face points
		for (int i = 0; i < faces.size(); i++)
		{
			for (int j = 0; j < faces[i]->edges.size(); j++)
			{
				if (faces[i]->edges[j]->edgepoint == NULL)
				{
					vec4 ep = vec4(0, 0, 0, 0);
					ep += faces[i]->edges[j]->v1->position;
					ep += faces[i]->edges[j]->v2->position;
					for (int k = 0; k < faces[i]->edges[j]->faces.size(); k++)
					{
						ep += faces[i]->edges[j]->faces[k]->facepoint->position;
					}
					ep = ep / (faces[i]->edges[j]->faces.size() + 2);
					faces[i]->edges[j]->edgepoint = new vertex(ep);
				}
			}
		}
		// Average the adjacent edge mid points and average the adjacent face points for every original point
		for (int i = 0; i < vertices.size(); i++)
		{
			// Face point average
			vec4 fpAverage = vec4(0, 0, 0, 0);
			for (int j = 0; j < vertices[i]->faces.size(); j++)
			{
				fpAverage += vertices[i]->faces[j]->facepoint->position;
			}
			fpAverage = fpAverage / vertices[i]->faces.size();
			// Edge mid point average - Note: This is not the average of the edge points.  This is the average of mid points of the edges.
			vec4 epAverage = vec4(0, 0, 0, 0);
			for (int k = 0; k < vertices[i]->edges.size(); k++)
			{
				epAverage += ((vertices[i]->edges[k]->v1->position + vertices[i]->edges[k]->v2->position) / 2);
			}
			epAverage = epAverage / vertices[i]->edges.size();

			int totalPoints = vertices[i]->faces.size();

			// Fix for working with topographs
			if (totalPoints < 3)
				totalPoints += 2;

			// Move original points to new positions using this formula
			vertices[i]->position = (fpAverage + (2 * epAverage) + (vertices[i]->position * (totalPoints - 3))) / totalPoints;
		}
		// Reorder the edges of each face to make it easier to create the new faces
		vertex* tmp1;
		vertex* tmp2;
		for (int i = 0; i < faces.size(); i++)
		{
			std::vector<edge*> tempEdges;
			for (int j = 0; j < faces[i]->vertices.size(); j++)
			{
				tmp1 = faces[i]->vertices[j];
				if (j == faces[i]->vertices.size() - 1)
				{
					tmp2 = faces[i]->vertices[0];
				}
				else
				{
					tmp2 = faces[i]->vertices[j + 1];
				}
				tempEdges.push_back(findCommonEdge(tmp1, tmp2));
			}
			faces[i]->edges = tempEdges;
		}
		// Add all the face points and edge points to mesh->vertices
		for (int i = 0; i < faces.size(); i++)
		{
			vertices.push_back(faces[i]->facepoint);
			for (int j = 0; j < faces[i]->edges.size(); j++)
			{
				if (faces[i]->edges[j]->added == false)
				{
					vertices.push_back(faces[i]->edges[j]->edgepoint);
					faces[i]->edges[j]->added = true;
				}
			}
		}
		// Clear current face ptrs and edge ptrs from the vertices
		for (int i = 0; i < vertices.size(); i++)
		{
			vertices[i]->faces.clear();
			vertices[i]->edges.clear();
		}
		// Prepare the new faces(and edges) for the mesh
		std::vector<face*> tempFaces;
		for (int i = 0; i < faces.size(); i++)
		{
			face* newFace = new face(faces[i]->vertices[0], faces[i]->edges[0]->edgepoint, faces[i]->facepoint, faces[i]->edges[3]->edgepoint, this);
			tempFaces.push_back(newFace);

			newFace = new face(faces[i]->edges[0]->edgepoint, faces[i]->vertices[1], faces[i]->edges[1]->edgepoint, faces[i]->facepoint, this);
			tempFaces.push_back(newFace);

			newFace = new face(faces[i]->facepoint, faces[i]->edges[1]->edgepoint, faces[i]->vertices[2], faces[i]->edges[2]->edgepoint, this);
			tempFaces.push_back(newFace);

			newFace = new face(faces[i]->edges[3]->edgepoint, faces[i]->facepoint, faces[i]->edges[2]->edgepoint, faces[i]->vertices[3], this);
			tempFaces.push_back(newFace);
		}
		//Delete old faces
		for (int i = 0; i < faces.size(); i++)
		{
			delete faces[i];
		}
		faces.clear();
		// Add the new faces(and edges)
		faces = tempFaces;
	}
}

edge*
mesh::findCommonEdge(vertex* v1, vertex* v2)
{
	for (int i = 0; i < v1->edges.size(); i++)
	{
		for (int j = 0; j < v2->edges.size(); j++)
		{
			if (v1->edges[i] == v2->edges[j])
			{
				return v1->edges[i];
			}
		}
	}
	return NULL;
}

vec4
mesh::getCurrentVertexPos()
{
	return currentVertex->position;
}

void
mesh::setCurrentVertexPos(vec4 v)
{
	currentVertex->position = v;
}

void
mesh::setCurrentVertex(vertex* v)
{
	currentVertex = v;
}

void
mesh::generateHeightField(int r, int c, int w, int d)
{
	int numRows = r;
	int numCols = c;
	int depth = d;

	std::vector<std::vector<vertex*>> grid;		// Prepare a grid
	grid.resize(numRows + 1);
	for (int i = 0; i <= numRows; i++)
	{
		grid[i].resize(numCols + 1);
	}

	// Fill grid with vertices with a random height and add them to mesh->vertices
	for (int i = 0; i <= numRows; i++)
	{
		for (int j = 0; j <= numCols; j++)
		{
			grid[i][j] = new vertex(vec4(i, j, rand() % depth, 1));
			vertices.push_back(grid[i][j]);
		}
	}

	// Create the new faces and add them to mesh->faces
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			faces.push_back(new face(grid[i][j], grid[i + 1][j], grid[i + 1][j + 1], grid[i][j + 1], this));
		}
	}
}

void
mesh::generateCube()
{
	vertices.push_back(new vertex(vec4(0, 0, 0, 1)));
	vertices.push_back(new vertex(vec4(1, 0, 0, 1)));
	vertices.push_back(new vertex(vec4(1, 1, 0, 1)));
	vertices.push_back(new vertex(vec4(0, 1, 0, 1)));
	vertices.push_back(new vertex(vec4(0, 0, 1, 1)));
	vertices.push_back(new vertex(vec4(1, 0, 1, 1)));
	vertices.push_back(new vertex(vec4(1, 1, 1, 1)));
	vertices.push_back(new vertex(vec4(0, 1, 1, 1)));

	faces.push_back(new face(vertices[0], vertices[1], vertices[2], vertices[3], this));
	faces.push_back(new face(vertices[1], vertices[5], vertices[6], vertices[2], this));
	faces.push_back(new face(vertices[5], vertices[4], vertices[7], vertices[6], this));
	faces.push_back(new face(vertices[4], vertices[0], vertices[3], vertices[7], this));
	faces.push_back(new face(vertices[4], vertices[5], vertices[1], vertices[0], this));
	faces.push_back(new face(vertices[3], vertices[2], vertices[6], vertices[7], this));
}

void
mesh::generatePyramid()
{
	vertices.push_back(new vertex(vec4(0, 0, 0, 1)));
	vertices.push_back(new vertex(vec4(1, 0, 0, 1)));
	vertices.push_back(new vertex(vec4(1, 1, 0, 1)));
	vertices.push_back(new vertex(vec4(0, 1, 0, 1)));
	vertices.push_back(new vertex(vec4(0.49, 0.49, 1, 1)));
	vertices.push_back(new vertex(vec4(0.51, 0.49, 1, 1)));
	vertices.push_back(new vertex(vec4(0.51, 0.51, 1, 1)));
	vertices.push_back(new vertex(vec4(0.49, 0.51, 1, 1)));

	faces.push_back(new face(vertices[0], vertices[1], vertices[2], vertices[3], this));
	faces.push_back(new face(vertices[1], vertices[5], vertices[6], vertices[2], this));
	faces.push_back(new face(vertices[5], vertices[4], vertices[7], vertices[6], this));
	faces.push_back(new face(vertices[4], vertices[0], vertices[3], vertices[7], this));
	faces.push_back(new face(vertices[4], vertices[5], vertices[1], vertices[0], this));
	faces.push_back(new face(vertices[3], vertices[2], vertices[6], vertices[7], this));
}

void
mesh::generateT()
{
	vertices.push_back(new vertex(vec4(0, 0, 0, 1)));
	vertices.push_back(new vertex(vec4(1, 0, 0, 1)));
	vertices.push_back(new vertex(vec4(1, 1, 0, 1)));
	vertices.push_back(new vertex(vec4(0, 1, 0, 1)));
	vertices.push_back(new vertex(vec4(0, 0, 1, 1)));
	vertices.push_back(new vertex(vec4(1, 0, 1, 1)));
	vertices.push_back(new vertex(vec4(1, 1, 1, 1)));
	vertices.push_back(new vertex(vec4(0, 1, 1, 1)));
	vertices.push_back(new vertex(vec4(-1, 1, 0, 1)));
	vertices.push_back(new vertex(vec4(-1, 2, 0, 1)));
	vertices.push_back(new vertex(vec4(0, 2, 0, 1)));
	vertices.push_back(new vertex(vec4(-1, 1, 1, 1)));
	vertices.push_back(new vertex(vec4(-1, 2, 1, 1)));
	vertices.push_back(new vertex(vec4(0, 2, 1, 1)));
	vertices.push_back(new vertex(vec4(1, 2, 0, 1)));
	vertices.push_back(new vertex(vec4(1, 2, 1, 1)));
	vertices.push_back(new vertex(vec4(2, 1, 0, 1)));
	vertices.push_back(new vertex(vec4(2, 2, 0, 1)));
	vertices.push_back(new vertex(vec4(2, 1, 1, 1)));
	vertices.push_back(new vertex(vec4(2, 2, 1, 1)));

	faces.push_back(new face(vertices[0], vertices[1], vertices[2], vertices[3], this));
	faces.push_back(new face(vertices[1], vertices[5], vertices[6], vertices[2], this));
	faces.push_back(new face(vertices[5], vertices[4], vertices[7], vertices[6], this));
	faces.push_back(new face(vertices[4], vertices[0], vertices[3], vertices[7], this));
	faces.push_back(new face(vertices[4], vertices[5], vertices[1], vertices[0], this));
	faces.push_back(new face(vertices[8], vertices[9], vertices[10], vertices[3], this));
	faces.push_back(new face(vertices[8], vertices[9], vertices[12], vertices[11], this));
	faces.push_back(new face(vertices[13], vertices[12], vertices[11], vertices[7], this));
	faces.push_back(new face(vertices[9], vertices[10], vertices[13], vertices[12], this));
	faces.push_back(new face(vertices[8], vertices[11], vertices[7], vertices[3], this));
	faces.push_back(new face(vertices[3], vertices[2], vertices[14], vertices[10], this));
	faces.push_back(new face(vertices[10], vertices[14], vertices[15], vertices[13], this));
	faces.push_back(new face(vertices[6], vertices[7], vertices[13], vertices[15], this));
	faces.push_back(new face(vertices[2], vertices[16], vertices[17], vertices[14], this));
	faces.push_back(new face(vertices[16], vertices[17], vertices[19], vertices[18], this));
	faces.push_back(new face(vertices[18], vertices[19], vertices[15], vertices[6], this));
	faces.push_back(new face(vertices[14], vertices[15], vertices[19], vertices[17], this));
	faces.push_back(new face(vertices[2], vertices[16], vertices[18], vertices[6], this));
}