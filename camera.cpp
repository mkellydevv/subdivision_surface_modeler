#include "camera.h"

camera::camera(float r, float phi, float theta){
	rot = identity3D();
	up = vec4(0, 1, 0, 1);
	eye = vec4(0, 0, r, 1);
	center = vec4(0, 0, 0, 0);
}

void
camera::increasePhi(float inc){
	rot = rot * rotation3D(vec3(0, 1, 0), inc);
}

void
camera::increaseTheta(float inc){
	rot = rot * rotation3D(vec3(1, 0, 0), inc);
}

void
camera::increaseRad(float inc){
	eye[2] = eye[2] + inc;
}

void
camera::setTopView(){
	rot = identity3D();
	up = vec4(0, 1, 0, 1);
	eye = vec4(0, 0, 6, 1);
}

void
camera::setSideView(){
	rot = rotation3D(vec3(1, 0, 0), 90);
	eye = vec4(0, 0, 6, 1);
}

void
camera::setCenter(vec4 v){
	center = v;
}

vec4
camera::getEye(){
	return rot * eye;
}

vec4
camera::getUp(){
	return rot * up;
}

vec4
camera::getCenter(){
	return rot * center;
}