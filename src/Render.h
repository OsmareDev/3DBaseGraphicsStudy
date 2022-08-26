#ifndef __RENDER__
#define __RENDER__

#pragma once

#include "Window.h"
#include "Model.h"
#include "Crono.h"

namespace Render {

	void Renderize();

	// lineas
	void dLine_Bresenham(int x_ini, int y_ini, int x_end, int y_end, const uint32_t& color);
	void dLine_DDA(int x_ini, int y_ini, int x_end, int y_end, const uint32_t& color);
	bool sunderCoen(int& x0, int& y0, int& x1, int& y1);
	int compute(int x, int y);
	
	// triangulos
}

#endif //__RENDER__