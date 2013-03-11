#include <stdafx.h>

#include "vector.h"

TVector3D toVector(const float fX, const float fY, const float fZ) {
    TVector3D MyVector;
    
    MyVector.fX = fX;
    MyVector.fY = fY;
    MyVector.fZ = fZ;
        
    return (MyVector);
}

TVector2D toVector(const float fX, const float fY) {
    TVector2D MyVector;
    
    MyVector.fX = fX;
    MyVector.fY = fY;
            
    return (MyVector);
}

TVector3D sumVectors(const TVector3D Vector0, const TVector3D Vector1) {
    TVector3D MyVector;
    
    MyVector.fX = Vector0.fX + Vector1.fX;
    MyVector.fY = Vector0.fY + Vector1.fY;
    MyVector.fZ = Vector0.fZ + Vector1.fZ;
    
    return (MyVector);
}

TVector2D sumVectors(const TVector2D Vector0, const TVector2D Vector1) {
    TVector2D MyVector;
    
    MyVector.fX = Vector0.fX + Vector1.fX;
    MyVector.fY = Vector0.fY + Vector1.fY;
        
    return (MyVector);
}

TVector3D subVectors(const TVector3D Vector0, const TVector3D Vector1) {
    TVector3D MyVector;
    
    MyVector.fX = Vector0.fX - Vector1.fX;
    MyVector.fY = Vector0.fY - Vector1.fY;
    MyVector.fZ = Vector0.fZ - Vector1.fZ;
    
    return (MyVector);
}

TVector2D subVectors(const TVector2D Vector0, const TVector2D Vector1) {
    TVector2D MyVector;
    
    MyVector.fX = Vector0.fX - Vector1.fX;
    MyVector.fY = Vector0.fY - Vector1.fY;
        
    return (MyVector);
}

TVector3D multVector(const TVector3D Vector0, const float fNum) {
    TVector3D MyVector;
    
    MyVector.fX = Vector0.fX * fNum;
    MyVector.fY = Vector0.fY * fNum;
    MyVector.fZ = Vector0.fZ * fNum;
    
    return (MyVector);
}

TVector2D multVector(const TVector2D Vector0, const float fNum) {
    TVector2D MyVector;
    
    MyVector.fX = Vector0.fX * fNum;
    MyVector.fY = Vector0.fY * fNum;
        
    return (MyVector);
}

float getVectorMagnitude(const TVector3D Vector) {
    float fMagnitude;
    
    fMagnitude = sqrt((Vector.fX * Vector.fX) + (Vector.fY * Vector.fY) + (Vector.fZ * Vector.fZ));
    
    return (fMagnitude);
}

TVector3D getUnitVector(const TVector3D Vector) {
    float fMagnitude;
    TVector3D MyVector;
    
    fMagnitude = getVectorMagnitude(Vector);
    
    MyVector.fX = Vector.fX / fMagnitude;
    MyVector.fY = Vector.fY / fMagnitude;
    MyVector.fZ = Vector.fZ / fMagnitude;
    
    return (MyVector);
}

TVector3D getPolygonNormal(const TVector3D Vector0, const TVector3D Vector1, const TVector3D Vector2) {
    TVector3D MyVector, V1, V2, V3;
    
    V1 = subVectors(Vector0, Vector1); 
    V2 = subVectors(Vector2, Vector1);  
    
    V3 = getCrossProduct(V1, V2);
    
    MyVector = getUnitVector(V3);
    
    return (MyVector);
}

float getInnerProduct(const TVector3D Vector0, const TVector3D Vector1) {
    float fProduct;
    
    fProduct = (Vector0.fX * Vector1.fX) + (Vector0.fY * Vector1.fY) + (Vector0.fZ * Vector1.fZ);

    return (fProduct);
}

TVector3D getCrossProduct(const TVector3D Vector0, const TVector3D Vector1) {
    TVector3D MyVector;
    
    MyVector.fX = (Vector0.fY * Vector1.fZ) - (Vector0.fZ * Vector1.fY);
    MyVector.fY = (Vector0.fZ * Vector1.fX) - (Vector0.fX * Vector1.fZ);
    MyVector.fZ = (Vector0.fX * Vector1.fY) - (Vector0.fY * Vector1.fX);
    
    return (MyVector);
}

float getPlaneDistance(const TVector3D Normal, const TVector3D Point) {
    float fDistance;
    
    fDistance = -getInnerProduct(Normal, Point);
        
    return (fDistance);    
}

float getPointsDistance(const TVector3D Point0, const TVector3D Point1) {
    TVector3D MyVector;
    float fDistance;
    
    MyVector.fX = Point1.fX - Point0.fX;
    MyVector.fY = Point1.fY - Point0.fY;
    MyVector.fZ = Point1.fZ - Point0.fZ;
    
    fDistance = getVectorMagnitude(MyVector);
    
    return (fDistance);
}

float getVectorsAngle(const TVector3D Vector0, const TVector3D Vector1) {
    float fAngle;
    TVector3D V0, V1;
    
    V0 = getUnitVector(Vector0);
    V1 = getUnitVector(Vector1);
    
    fAngle = acosf(getInnerProduct(V0, V1));
    
    return (fAngle);
}