#ifndef _vector_h_
#define _vector_h_

typedef struct _Vector {
    float x;
    float y;
    float z;
} Vector, *PVector;

Vector toVector(float x, float y, float z);

Vector sumVectors(PVector Vector1, PVector Vector2);
Vector subVectors(PVector Vector1, PVector Vector2);
Vector multVector(PVector Vector1, GLfloat Value);

GLfloat getVectorMaginitude(PVector Normale);
Vector getNormalizedVector(PVector Normale);
Vector getVectorsNormale2(PVector Vector1, PVector Vector2);
Vector getVectorsNormale3(PVector Vector1, PVector Vector2, PVector Vector3);
GLfloat getVectorsDot(PVector Vector1, PVector Vector2);
GLfloat getPlaneDistance(PVector Origin, PVector Point);
GLfloat getPointsDistance(PVector Point1, PVector Point2);
GLdouble getVectorsAngle(PVector Vector1, PVector Vector2); 

#endif