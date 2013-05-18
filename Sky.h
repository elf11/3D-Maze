#ifndef _SKY_H_
#define _SKY_H_

#include "TargaImage.h"
#include "Math.h"

class TargaImage;

class Sky { 
	
	enum {
		SKY_TOP,
		SKY_BOTTOM,
		SKY_FRONT,
		SKY_BACK,
		SKY_LEFT,
		SKY_RIGHT
	};
	
	public: 
		Sky();
		virtual ~Sky();
		bool Init();
		void LoadTexture();
		void Render(Vector3 Pos);
		void Release();
	private:
		float m_size;
		unsigned int skyobj[6];		// 6 texture objects
};

#endif 