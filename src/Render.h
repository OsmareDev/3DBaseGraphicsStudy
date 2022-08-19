#ifndef __RENDER__
#define __RENDER__

#pragma once

#include "Window.h"
#include "Model.h"

namespace Render {

	void dLine_Bresenham(int x_ini, int y_ini, int x_end, int y_end, uint32_t color);
	bool sunderCoen(int& x0, int& y0, int& x1, int& y1);
	int compute(int x, int y);
	void Renderize();

}

#endif //__RENDER__