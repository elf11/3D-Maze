//clasa camera
#pragma once
#include "Vector3D.h"


class Camera{
	public:
		Camera();
		~Camera();

		void init();
		void render(int cam, float x, float y, float r, int size);

		private:
		Vector3D forward;
		Vector3D up;
		Vector3D right;
		Vector3D position;
};