#ifndef __MODEL_H__
#define __MODEL_H__

#pragma once

#include "geometry.h"

class Model {
private:
	std::vector<Vec3f> verts_;
	std::vector<std::vector<Vec3i> > faces_;
	std::vector<Vec3f> norms_;
	std::vector<Vec2f> uv_;
public:
	Model(const char* filename);
	~Model();
	int nverts();
	int nfaces();
	Vec3f norm(int iface, int nvert);
	Vec3f vert(int i);
	Vec2i uv(int iface, int nvert);
	std::vector<int> face(int idx);
};

#endif //__MODEL_H__