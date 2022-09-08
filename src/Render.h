#ifndef __RENDER__
#define __RENDER__

#pragma once

#include "Window.h"

namespace Render {
	
	void Renderize();

	// Lineas
	void bresenham(const Line& l, const uint32_t& color);
	void dda(const Line& l, const uint32_t& color);
	inline void Draw(const Line& l, const uint32_t& color) { bresenham(l, color); };

	bool sunderCoen(int& x0, int& y0, int& x1, int& y1);
	int compute(const int& x, const int& y);

	// Triangle
	void Clamp_bBox(BoundingBox& b);
	void Draw(Triangle& t, const uint32_t& color, bool wireframe = false);
}

#endif //__RENDER__