#include "stdafx.h"
#include <string>
#include <string.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <iostream>
#include <fstream>
#include "Includes\glee\GLee.h"
#include "glut.h"
#include "Sky.h"
#include "Math.h"
#include "TargaImage.h"
#include "Includes\OpenGL.h"

using namespace std;

//#define GL_CLAMP_TO_EDGE 0x812F

Sky::Sky(){
	m_size = 50.0f;
	for (int idx = 0; idx < 6; idx++)
		skyobj[idx] = 0;
}

Sky::~Sky(){
	Release();
}

bool Sky::Init(){
	
	// enable 2D texturing
	glEnable(GL_TEXTURE_2D);
	
	TargaImage skybox;
	
	glPushMatrix();
	//  fisrts right
	skybox.Load("Side1.tga");
	glGenTextures(1, &skyobj[SKY_RIGHT]);
	// bind the texture object
	glBindTexture(GL_TEXTURE_2D,  skyobj[SKY_RIGHT]);
	// minimum required to set the min and mag texture filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// now that the texture object is bound, specify a texture for it
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, skybox.GetWidth(), skybox.GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, skybox.GetImage());
	
	skybox.Release();
	
	// second  bottom 
	skybox.Load("Side2.tga");
	
	glGenTextures(1, &skyobj[SKY_BOTTOM]);
	// bind the texture object
	glBindTexture(GL_TEXTURE_2D,  skyobj[SKY_BOTTOM]);
	// minimum required to set the min and mag texture filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// now that the texture object is bound, specify a texture for it
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, skybox.GetWidth(), skybox.GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, skybox.GetImage());
	skybox.Release();
	

	// third left side
	skybox.Load("Side3.tga");
	
	glGenTextures(1, &skyobj[SKY_LEFT]);
	// bind the texture object
	glBindTexture(GL_TEXTURE_2D,  skyobj[SKY_LEFT]);
	// minimum required to set the min and mag texture filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// now that the texture object is bound, specify a texture for it
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, skybox.GetWidth(), skybox.GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, skybox.GetImage());
	skybox.Release();
	


	//  fourth front side 
	skybox.Load("Side4.tga");
	
	glGenTextures(1, &skyobj[SKY_FRONT]);
	// bind the texture object
	glBindTexture(GL_TEXTURE_2D,  skyobj[SKY_FRONT]);
	// minimum required to set the min and mag texture filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// now that the texture object is bound, specify a texture for it
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, skybox.GetWidth(), skybox.GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, skybox.GetImage());
	skybox.Release();
	


	//fifth 
	skybox.Load("Side5.tga");
	
	glGenTextures(1, &skyobj[SKY_BACK]);
	// bind the texture object
	glBindTexture(GL_TEXTURE_2D,  skyobj[SKY_BACK]);
	// minimum required to set the min and mag texture filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// now that the texture object is bound, specify a texture for it
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, skybox.GetWidth(), skybox.GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, skybox.GetImage());
	skybox.Release();
	


	// sixth
	skybox.Load("Side6.tga");
	
	glGenTextures(1, &skyobj[SKY_TOP]);
	// bind the texture object
	glBindTexture(GL_TEXTURE_2D,  skyobj[SKY_TOP]);
	// minimum required to set the min and mag texture filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// now that the texture object is bound, specify a texture for it
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, skybox.GetWidth(), skybox.GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, skybox.GetImage());
	skybox.Release();
	
	glPopMatrix();

	return true;



}


void Sky::Render(Vector3 cameraPos){
		
		Init();
		glPushMatrix();
		glTranslatef(cameraPos.x, cameraPos.y, cameraPos.z);
		
		glPushAttrib(GL_FOG_BIT | GL_DEPTH_BUFFER_BIT | GL_LIGHTING_BIT);
		glDisable(GL_DEPTH_TEST);

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	
		glColor3f(0.16f, 0.789f, 0.85f);
		
		GLfloat skyColor[] = { 0.0f, 0.0f, 0.5f, 1.0f };
		GLfloat skySpecular[] = { 0.0f, 0.0f, 0.5f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, skyColor);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, skySpecular);

		 //Top
		glBindTexture(GL_TEXTURE_2D, skyobj[SKY_TOP]);
		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-m_size, m_size, -m_size);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(m_size, m_size, -m_size);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(m_size, m_size, m_size);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-m_size, m_size, m_size);
		glEnd();
		glDeleteTextures(1,&skyobj[SKY_TOP]);
		

		// Bottom
		glBindTexture(GL_TEXTURE_2D, skyobj[SKY_BOTTOM]);
		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(m_size, -m_size, -m_size);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-m_size, -m_size, -m_size);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-m_size, -m_size, m_size);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(m_size, -m_size, m_size);
		glEnd();
		glDeleteTextures(1,&skyobj[SKY_BOTTOM]);
		// Front
		glBindTexture(GL_TEXTURE_2D, skyobj[SKY_FRONT]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-m_size, -m_size, -m_size);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(m_size, -m_size, -m_size);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(m_size, m_size, -m_size);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-m_size, m_size, -m_size);
		glEnd();
		glDeleteTextures(1,&skyobj[SKY_FRONT]);
		glBindTexture(GL_TEXTURE_2D, skyobj[SKY_BACK]);
	//	 Back
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(m_size, -m_size, m_size);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-m_size, -m_size, m_size);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-m_size, m_size, m_size);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(m_size, m_size, m_size);
		glEnd();
		glDeleteTextures(1,&skyobj[SKY_BACK]);
	//	 Right
		glBindTexture(GL_TEXTURE_2D, skyobj[SKY_RIGHT]);
		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(m_size, -m_size, m_size);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(m_size, m_size, m_size);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(m_size, m_size, -m_size);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(m_size, -m_size, -m_size);
		glEnd();
		glDeleteTextures(1,&skyobj[SKY_RIGHT]);
	//	 Left
		glBindTexture(GL_TEXTURE_2D, skyobj[SKY_LEFT]);
		glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-m_size, -m_size, -m_size);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-m_size, m_size, -m_size); 
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-m_size, m_size, m_size);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-m_size, -m_size, m_size);
		glEnd();
		glDeleteTextures(1,&skyobj[SKY_LEFT]);
	
		glEnable(GL_DEPTH_TEST);
	
	glPopMatrix();

	Release();

}

void Sky::Release(){
	for (int i = 0; i < 6; ++i)
		glDeleteTextures(6, &skyobj[i]);
	
}

