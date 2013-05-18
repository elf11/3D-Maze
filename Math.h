#ifndef _MATH_H_
#define _MATH_H_

#define DEG2RAD(x) ((x)*0.0174532925f)

#include "Includes\OpenGL.h"

class Vector2
{
	public:
		Vector2(GLfloat xx = 0.f, GLfloat yy = 0.f): x(xx), y(yy){};
		GLfloat x;
		GLfloat y;
};

class Vector3
{
	public:
		Vector3(GLfloat xx = 0.f, GLfloat yy = 0.f, GLfloat zz = 0.f): x(xx), y(yy), z(zz){};
		void Set(GLfloat x, GLfloat y, GLfloat z);
		Vector3 operator+(Vector3 vect);
		Vector3 operator-(Vector3 vect);
		Vector3 operator*(GLfloat alpha);

		GLfloat x;
		GLfloat y;
		GLfloat z;
};

class Vector4
{
	public:
		Vector4(GLfloat xx = 0.f, GLfloat yy = 0.f, GLfloat zz = 0.f, GLfloat ww = 0.f): x(xx), y(yy), z(zz), w(ww){};
		GLfloat x;
		GLfloat y;
		GLfloat z;
		GLfloat w;
};

class Color
{
	public:
		Color(GLfloat rr = 0.f, GLfloat gg = 0.f, GLfloat bb = 0.f): r(rr), g(gg), b(bb){};
		GLfloat r;
		GLfloat g;
		GLfloat b;
};

void ComputeFrontAndRight(Vector3& front, Vector3& right, GLfloat pitch, GLfloat heading);

#endif