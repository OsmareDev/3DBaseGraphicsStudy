#ifndef __CAMERA__
#define __CAMERA__

#pragma once

#include "Model.h"

class Void_Camera {
	protected:
		int window_width = 1280, window_height = 720;
		float z_near = 1.f, z_far = 1000.f;
		float z_buffer = 1.f;
		float fov = 90;

		Matrix To_ViewPort{ 4,4 };
		Matrix Projection{ 4,4 };
		Matrix World_Camera{ 4,4 };

	public:
		Void_Camera() { SetMatrix(); };
		void SetMatrix();
		void TransformTriangle(Triangle& t);
		Vec2i to_viewport(Vec3f v);
};

Vec3f m2v(Matrix m);
Matrix v2m(Vec3f v);

#endif //__CAMERA__