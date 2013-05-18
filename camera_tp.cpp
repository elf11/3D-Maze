#include "camera.h"
#include <stdio.h>

Camera::Camera(){
}
Camera::~Camera(){
}

void Camera::init(){
	position = Vector3D(0,0,3);
	forward = Vector3D(0,0,-1);
	up = Vector3D(0,1,0);
	right = Vector3D(1,0,0);
}

void Camera::render(int cam, float x, float y, float r, int size){
	Vector3D center, location;
	//0 - up, 1 - first person, 2 - third person
	switch (cam){
		case 0:
			//depends on the size of the labirinth
			gluLookAt(	0, 0, size + 5, 
				0, 0, 0,
				0, 1, 0);
			break;
		case 1:
			//calculare the directions based on player position and direction of movement
			center = Vector3D(0, 0, 0);
			location = Vector3D(0, 0, 0);
			center.x = x - sin((3.14 * r) / 180);
			center.y = y + cos((3.14 * r) / 180);
			location.x = x - sin((3.14 * r) / 180) * 0.4;
			location.y = y + cos((3.14 * r) / 180) * 0.4;
			gluLookAt(location.x, location.y, 0, 
				center.x, center.y, 0,
				0, 0, 1);
			break;
		case 2:
			//calculare the directions based on player position and direction of movement
			center = Vector3D(0, 0, 0);
			location = Vector3D(0, 0, 0);
			center.x = x - sin((3.14 * r) / 180);
			center.y = y + cos((3.14 * r) / 180);
			location.x = x + sin((3.14 * r) / 180) * 3;
			location.y = y - cos((3.14 * r) / 180) * 3;
			gluLookAt(location.x, location.y, 1, 
				center.x, center.y, 0.1,
				0, 0, 1);
			break;
	}
}