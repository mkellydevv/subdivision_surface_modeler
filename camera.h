#ifndef CAMERA_H
#define CAMERA_H

#include "algebra3.h"

// A class for storing camera transforms
class camera
{
private:
	mat4 rot;				// Current rotation matrix 
	vec4 up;				// Current up vector
	vec4 eye;				// Current eye/camera position
	vec4 center;			// Current camera center
public:
	camera(float r, float phi, float theta);	// Constructor
	void increasePhi(float inc);		// Increase the phi angle. Note a negative value will decrease it. 
	void increaseTheta(float inc);		// Increase the theta angle
	void increaseRad(float inc);		// Increase the radius
	void setTopView();					// Set camera to an overhead view of the mesh
	void setSideView();					// Set camera to a side view of the mesh
	void setCenter(vec4 v);				// Set center
	vec4 getUp();						// get the current up vector
	vec4 getEye();						// get the current eye position
	vec4 getCenter();
};

#endif