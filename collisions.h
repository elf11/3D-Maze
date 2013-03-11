#ifndef _collisions_h_
#define _collisions_h_

#include "vector.h"

#define CLS_BEHIND 0;
#define CLS_INTERSECTS 1;
#define CLS_FRONT 2;

TVector3D getClosestPointOnLine(TVector3D vLineStart, TVector3D vLineEnd, TVector3D vPoint);
int getSphereRelDisposition(TVector3D& vCenter, TVector3D& vNormal, TVector3D& vPoint, float fRadius, float& fDistance);
BOOL isEdgeCollision(TVector3D& vCenter, PTVector3D arr_vPolygon, int nVertexCount, float fRadius);    
BOOL isInsidePolygon(TVector3D vIntersection, PTVector3D arr_vPoly, int nVertexCount);
TVector3D getCollisionOffset(TVector3D& vNormal, float fRadius, float fDistance);

#endif