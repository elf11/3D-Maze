#include <stdafx.h>
#include <math.h>

#include "vector.h"

Vector toVector(GLfloat x, GLfloat y, GLfloat z) {
    Vector MyVector;
    
    MyVector.x = x;
    MyVector.y = y;
    MyVector.z = z;
    
    return (MyVector);
}

// Base functions
Vector sumVectors(PVector Vector1, PVector Vector2) {
    Vector MyVector;
    
    MyVector.x = Vector1->x + Vector2->x;
    MyVector.y = Vector1->y + Vector2->y;
    MyVector.z = Vector1->z + Vector2->z;
    
    return (MyVector);
}

Vector subVectors(PVector Vector1, PVector Vector2) {
    Vector MyVector;

    MyVector.x = Vector1->x - Vector2->x;
    MyVector.y = Vector1->y - Vector2->y;
    MyVector.z = Vector1->z - Vector2->z;
    
    return (MyVector);
}

Vector multVectors(PVector Vector1, GLfloat Val) {
    Vector MyVector;

    MyVector.x = Vector1->x * Val;
    MyVector.y = Vector1->y * Val;
    MyVector.z = Vector1->z * Val;
    
    return (MyVector);
}

/*Vector multVector(PVector Vector, GLfloat Value) {
    Vector MyVector;
    
    MyVector.x = Vector->x * Value;
    MyVector.y = Vector->y * Value;
    MyVector.z = Vector->z * Value;

    return (MyVector);
}*/

// Advance functions

GLfloat getVectorMaginitude(PVector Normale) {
    return(sqrt((Normale->x * Normale->x) + (Normale->y * Normale->y) + (Normale->z * Normale->z)));
}

Vector getNormalizedVector(PVector Normale) {
    GLfloat Magnitude;
    Vector MyVector;

    Magnitude = getVectorMaginitude(Normale);

    MyVector.x = Normale->x / Magnitude;
    MyVector.y = Normale->y / Magnitude;
    MyVector.z = Normale->z / Magnitude;
    
    return (MyVector);
}

Vector getVectorsNormale3(PVector Vector1, PVector Vector2, PVector Vector3) {
    Vector V1, V2, V3, MyVector;
    
    V1 = subVectors(Vector1, Vector2);
    V2 = subVectors(Vector2, Vector3);
    V3 = getVectorsNormale2(&V1, &V2);
    MyVector = getNormalizedVector(&V3);
    
    return (MyVector);
}

GLfloat getVectorsDot(PVector Vector1, PVector Vector2) {
    return ((Vector1->x * Vector2->x) + (Vector1->y * Vector2->y) + (Vector1->z * Vector2->z));
}

Vector getVectorsNormale2(PVector Vector1, PVector Vector2) {
    Vector MyVector;

    MyVector.x = (Vector1->y * Vector2->z) - (Vector1->z * Vector2->y);
    MyVector.y = (Vector1->z * Vector2->x) - (Vector1->x * Vector2->z);
    MyVector.z = (Vector1->x * Vector2->y) - (Vector1->y * Vector2->x);
    
    return (MyVector);
}   

GLfloat getPlaneDistance(PVector Origin, PVector Point) {
    return (-(Origin->x * Point->x + Origin->y * Point->y + Origin->z * Point->z));
}

GLfloat getPointsDistance(PVector Point1, PVector Point2) {
    return (sqrt((Point2->x - Point1->x) * (Point2->x - Point1->x) + (Point2->y - Point1->y) * (Point2->y - Point1->y) + (Point2->z - Point1->z) * (Point2->z - Point1->z)));
}

GLdouble getVectorsAngle(PVector Vector1, PVector Vector2) {
    Vector V1, V2;
    
    V1 = getNormalizedVector(Vector1);
    V2 = getNormalizedVector(Vector2);

    return (acos(getVectorsDot(&V1, &V2)));
}
