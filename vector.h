#ifndef _vector_h_
#define _vector_h_

#include <math.h>

typedef struct _TVector3D {
    float fX;
    float fY;
    float fZ;
} TVector3D, *PTVector3D;

typedef struct _TVector2D {
    float fX;
    float fY;
} TVector2D, *PTVector2D;

TVector3D toVector(const float fX, const float fY, const float fZ);
TVector2D toVector(const float fX, const float fY);

TVector3D sumVectors(const TVector3D Vector0, const TVector3D Vector1);
TVector2D sumVectors(const TVector2D Vector0, const TVector2D Vector1);

TVector3D subVectors(const TVector3D Vector0, const TVector3D Vector1);
TVector2D subVectors(const TVector2D Vector0, const TVector2D Vector1);

TVector3D multVector(const TVector3D Vector0, const float fNum);
TVector2D multVector(const TVector2D Vector0, const float fNum);

float getVectorMagnitude(const TVector3D Vector);
TVector3D getUnitVector(const TVector3D Vector);

TVector3D getPolygonNormal(const TVector3D Vector0, const TVector3D Vector1, const TVector3D Vector2);

float getPlaneDistance(const TVector3D Normal, const TVector3D Point);
float getPointsDistance(const TVector3D Point0, const TVector3D Point1);

float getInnerProduct(const TVector3D Vector0, const TVector3D Vector1);
TVector3D getCrossProduct(const TVector3D Vector0, const TVector3D Vector1);

float getVectorsAngle(const TVector3D Vector0, const TVector3D Vector1);



#endif