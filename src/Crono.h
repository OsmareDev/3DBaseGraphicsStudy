#ifndef __CRONO__
#define __CRONO__

#pragma once

#include <chrono>
#include <Windows.h>

class Crono {
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float> duration;

public:
	Crono()
	{
		start = std::chrono::high_resolution_clock::now();
	}

	~Crono()
	{
		end = std::chrono::high_resolution_clock::now();
		duration = end - start;

		wchar_t buffer[256];
		char vOutChar[256];

		// multiplicar por 1000.0f para milisegundos
		_gcvt_s(vOutChar, sizeof(vOutChar), duration.count(), 8);
		mbstowcs_s(NULL, buffer, sizeof(buffer) / 2, vOutChar, sizeof(vOutChar));


		MessageBoxW(nullptr, buffer, buffer, MB_OK);
	}
};



#endif //__CRONO__
