#include <stdafx.h>

#include "collisions.h"

TVector3D getClosestPointOnLine(TVector3D vLineStart, TVector3D vLineEnd, TVector3D vPoint) {
    TVector3D vVector1, vVector2, vVector3, vClosestPoint;
    float d, t;
  
    vVector1 = subVectors(vPoint, vLineStart);
  
    vVector2 = subVectors(vLineEnd, vLineStart);
    vVector2 = getUnitVector(vVector2);
  
    d = getPointsDistance(vLineStart, vLineEnd);
    t = getInnerProduct(vVector1, vVector2);

    if (t <= 0) {
        return vLineStart;
    }  
    if (t >= d) {
        return vLineEnd;
    }
   
    vVector3 = multVector(vVector2, t);
    vClosestPoint = sumVectors(vLineStart, vVector3);
  
    return vClosestPoint;
}

int getSphereRelDisposition(TVector3D& vCenter, TVector3D& vNormal, TVector3D& vPoint, 
    float fRadius, float& fDistance) {
    float d;
  
    d = getPlaneDistance(vNormal, vPoint);
    fDistance = getInnerProduct(vCenter, vNormal) + d;
  
    if (fabs(fDistance) < fRadius) {
        return CLS_INTERSECTS;
    } else {
        if (fDistance >= fRadius) {
            return CLS_FRONT;
        }
    }
    
    return CLS_BEHIND;
}

BOOL isEdgeCollision(TVector3D& vCenter, PTVector3D arr_vPolygon,
                     int nVertexCount, float fRadius) {
    TVector3D vPoint;
    int i;
    float fDistance;
  
    for(i = 0; i < nVertexCount; ++i) {
        vPoint = getClosestPointOnLine(arr_vPolygon[i], arr_vPolygon[(i + 1) % nVertexCount], vCenter);
        fDistance = getPointsDistance(vPoint, vCenter);
    
        if (fDistance < fRadius) {
            return TRUE;
        }    
    }
    
    return FALSE;
}

BOOL isInsidePolygon(TVector3D vIntersection, PTVector3D arr_vPoly, int nVertexCount) {
    double fAngle = 0.0;
    TVector3D vA, vB;
  
    for (int i = 0; i < nVertexCount; ++i) {  
        vA = subVectors(arr_vPoly[i], vIntersection);  
        vB = subVectors(arr_vPoly[(i + 1) % nVertexCount], vIntersection);
    
        fAngle += getVectorsAngle(vA, vB);  
    }
  
    if (fAngle >= (0.99f * (2.0f * 3.14f))) {
        return TRUE;
    }  
    
    return FALSE;
}

TVector3D getCollisionOffset(TVector3D& vNormal, float fRadius, float fDistance) {
    TVector3D vOffset;
    float fDistanceOver;
  
    if (fDistance > 0) {
        fDistanceOver = fRadius - fDistance;
    } else {
        fDistanceOver = -(fRadius + fDistance);
    }
        
    vOffset = multVector(vNormal, fDistanceOver);

    return vOffset;
}