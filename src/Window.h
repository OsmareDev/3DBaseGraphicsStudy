#ifndef __WINDOW__
#define __WINDOW__

#pragma once

#include <windows.h>
#include <string>


class Window {
public:
	Window(const Window&) = delete;

	static Window& GetInstance() {
		return main_window;
	}

	// Getters
	const int& GetWidth() const { return window_width; }
	const int& GetHeight() const { return window_height; }
	const std::wstring& GetTitle() const { return window_title; }

	// Setters
	void SetWidth(const int& width) { window_width = width; }
	void SetHeight(const int& height) { window_height = height; }
	void SetResolution(const int& w, const int& h) { window_width = w, window_height = h; }
	void SetTitle(const std::wstring& title) { window_title = title; }

	// Friends
	friend LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// Functions
	void SetRender(void(*function)()) { Render = function; };
	void StartLoop();
	void PaintPixel(const int& desplazamiento, const uint32_t& color) const;
	
private:
	Window();
	void DefineClass();
	void WindowCreate();
	void RunningLoop();
	void RecalculateMemory();

	static Window main_window;

	void(*Render)();

	int window_width = 1280, window_height = 720;
	std::wstring window_title = L"Void Engine";
	bool window_running = false;
	int bufferSize;
	void* memory;

	// WIN32
	const wchar_t* CLASS_NAME = L"VoidEngine Window";
	WNDCLASS wc = { };
	HWND window;
	HINSTANCE hInstance;
	int nCmdShow; 
	HDC hdc;
	BITMAPINFO bitmap_info;
};

#endif //__WINDOW__