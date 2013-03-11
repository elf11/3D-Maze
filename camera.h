#ifndef _camera_h_
#define _camera_h_

#include "vector.h"

extern int g_nVertexCount;

typedef struct _TVertex {
    TVector3D     arr_vTriangle[3];
    TVector2D     arr_vTexture[3];
    TVector3D     vNormal;
} TVertex, *PTVertex;

typedef class _CCamera {
    public:
        float m_fPosX;
        float m_fPosY;
        float m_fPosZ;
        float m_fPitch;
        float m_fYaw;
        float m_fRoll;
        float m_fSpeedMult;
        /**/
        _CCamera(float x, float y, float z);
        ~_CCamera();
        
        void reset();
		void set(float x, float y, float z);
        
        void moveForward(float fSpeed);
        void moveStrafe(float fSpeed);
        void moveUp(float fSpeed);
        
        void rotateRPY(float fRoll, float fPitch, float fYaw);
        
        void mouseLook();
        void checkCollision(PTVertex arrVertices);
        
        void adjustPlayerControls(int nNewControlFlag);
    private:
        float m_fRadius;
        int   nPlayerControls;
        /**/
        void correctPosition(TVector3D vNewPosition);
    protected:
} CCamera, *PCCamera;

#endif