#include <stdafx.h>
#include <math.h>

#include "camera.h"
#include "collisions.h"

POINT curPosition;
float offsetX = 0, offsetY = 0;

int g_nVertexCount = 0;

void _CCamera::reset() {
    m_fPosX       = 0.5f;
    m_fPosY       = 1.0f;
    m_fPosZ       = -0.1f;
    m_fPitch      = 0.0f;
    m_fYaw        = 180.0f;
    m_fRoll       = 0.0f;
    m_fSpeedMult  = 1.0f;
    m_fRadius     = 0.01f;
    nPlayerControls = 2;
}

void _CCamera::set(float x, float y, float z) {
    m_fPosX       = x;
    m_fPosY       = y;
    m_fPosZ       = z;
    m_fPitch      = 0.0f;
    m_fYaw        = 180.0f;
    m_fRoll       = 0.0f;
    m_fSpeedMult  = 1.0f;
    m_fRadius     = 0.1f;
    nPlayerControls = 2;
}

void _CCamera::moveForward(float fSpeed) {
    if (nPlayerControls == 2) {
        this->m_fPosX += fSpeed * m_fSpeedMult * (float)sin(-(this->m_fYaw) * 3.14 / 180.0) * (float)cos(this->m_fPitch * 3.14 / 180.0);
        this->m_fPosY += fSpeed * m_fSpeedMult * (float)sin(this->m_fPitch * 3.14 / 180.0);
		if (this->m_fPosY < 0.0)
		{
			this->m_fPosY = 0.2;
		}
		if (this->m_fPosY > 1.0)
		{
			this->m_fPosY = 0.9;
		}
        this->m_fPosZ += fSpeed * m_fSpeedMult * (float)cos(this->m_fYaw * 3.14 / 180.0) * (float)cos(this->m_fPitch * 3.14 / 180.0);
    }
}

void _CCamera::moveStrafe(float fSpeed) {
    if (nPlayerControls == 2) {
        this->m_fPosX += fSpeed * (float)cos((this->m_fYaw) * 3.14 / 180.0);
        this->m_fPosZ += fSpeed * (float)sin((this->m_fYaw) * 3.14 / 180.0);
    }    
}

void _CCamera::moveUp(float fSpeed) {
    if (nPlayerControls == 2) {
        //this->m_fPosY += fSpeed;  // No view angle dependancies
		/*if (this->m_fPosY < 0.1)
		{
			this->m_fPosY = 0.2;
		}
		if (this->m_fPosY > 1.0)
		{
			this->m_fPosY = 0.9;
		}*/
    }    
}

void _CCamera::rotateRPY(float fRoll, float fPitch, float fYaw) {
    if (nPlayerControls >= 1) {
        this->m_fRoll += fRoll;
        if (this->m_fRoll > 20.0f) {
            this->m_fRoll = 20.0f;
        } else {
            if (this->m_fRoll < -20.0f) {
                this->m_fRoll = -20.0f;
            }
        }
            
        this->m_fPitch += fPitch;
        if (this->m_fPitch > 90.0f) {
            this->m_fPitch = 90.0f;
        } else {
            if (this->m_fPitch < -90.0f) {
                this->m_fPitch = -90.0f;
            }
        }        
        
        this->m_fYaw += fYaw;
        if (this->m_fYaw > 360.0f) {
            this->m_fYaw = 0.0f;
        } else {
            if (this->m_fYaw < -360.0f) {
                this->m_fYaw = 0.0f;
            }
        }
    }    
}

void _CCamera::mouseLook() {
    GetCursorPos(&curPosition);
    SetCursorPos(320, 240);
    
    offsetX = ((float)curPosition.x - 320.0f) * 0.1f;
    offsetY = ((float)curPosition.y - 240.0f) * 0.1f;
    
    this->rotateRPY(0.0f, offsetY, offsetX);
    
    offsetX = 0;
    offsetY = 0;
}

//modificati pe aici daca vreti sa porneasca de la o anumita inaltime pe Y. 
void _CCamera::correctPosition(TVector3D vNewPosition) {
    this->m_fPosX = vNewPosition.fX;
	this->m_fPosY = 0.5;
    //this->m_fPosY = vNewPosition.fY;
	if (this->m_fPosY < 0.0)
		{
			this->m_fPosY = 0.2;
		}
	if (this->m_fPosY > 1.0)
		{
			this->m_fPosY = 0.9;
		}
    this->m_fPosZ = vNewPosition.fZ;
}

void _CCamera::checkCollision(PTVertex arrVertices) {
    int i;
    TVector3D vCamPos, vOffset, vIntersection;
    float fDistance = 0.0f;
    BOOL bIsEdgeCollision;
    
    vCamPos = toVector(this->m_fPosX, this->m_fPosY, this->m_fPosZ);
            
    for (i = 0; i < g_nVertexCount + 1; ++i) {
        if (getSphereRelDisposition(vCamPos,
                                    arrVertices[i].vNormal,
                                    arrVertices[i].arr_vTriangle[0],
                                    this->m_fRadius,
                                    fDistance) == 1) {
            vOffset = multVector(arrVertices[i].vNormal, fDistance);
            vIntersection = subVectors(vCamPos, vOffset);
            
            bIsEdgeCollision = isEdgeCollision(vCamPos, arrVertices[i].arr_vTriangle, 3, this->m_fRadius / 4);
            if (isInsidePolygon(vIntersection, arrVertices[i].arr_vTriangle, 3) || bIsEdgeCollision) {
                vOffset = getCollisionOffset(arrVertices[i].vNormal, this->m_fRadius, fDistance);   
                vCamPos = sumVectors(vCamPos, vOffset);
            }    
        }
    }      

    correctPosition(vCamPos);
}

void _CCamera::adjustPlayerControls(int nNewControlFlag) {
    this->nPlayerControls = nNewControlFlag;
}

_CCamera::_CCamera(float x, float y, float z) {
	set(x, y, z);
}

_CCamera::~_CCamera() {

}