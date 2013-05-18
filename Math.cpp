#include "stdafx.h"
#include "Math.h"
#include <math.h>

void ComputeFrontAndRight(Vector3& front, Vector3& right, GLfloat pitch, GLfloat heading)
{
	GLfloat sinP = sinf(DEG2RAD(pitch));
	GLfloat cosP = cosf(DEG2RAD(pitch));

	GLfloat sinH = sinf(DEG2RAD(heading));
	GLfloat cosH = cosf(DEG2RAD(heading));

	right.x = cosP;
	right.y = 0;
	right.z = sinP;

	front.x = -sinP * cosH;
	front.y = sinH;
	front.z = cosP * cosH;
}

void Vector3::Set(GLfloat xx, GLfloat yy, GLfloat zz){
	x = xx; y = yy; z = zz;
}

Vector3 Vector3::operator+(Vector3 vect){
	Vector3 result;
	result.Set(x + vect.x, y + vect.y, z + vect.z);
	return result;
}

Vector3 Vector3::operator-(Vector3 vect){
	Vector3 result;
	result.Set(x - vect.x, y - vect.y, z - vect.z);
	return result;
}

Vector3 Vector3::operator*(GLfloat alfa){
	Vector3 result;
	result.Set(x*alfa, y*alfa, z*alfa);
	return result;
}

