#pragma once
#include <d3d11_1.h>
#include "resource.h"


class Window {
private:
	HINSTANCE	_hInst;
	HWND		_hWnd;
	
	int			_cmdShow;
	LPCWSTR		_windowName;
	LPCWSTR		_windowClass;

	RECT		_size = { 0, 0, 1920, 1200 };
	UINT _height;
	UINT _width;

public:
	Window(HINSTANCE hInstance, int nCmdShow, LPCWSTR windowName, LPCWSTR windowClass);
	~Window();

	HRESULT Initialise();
	void Destroy();

	void ShowWind() {
		ShowWindow(_hWnd, _cmdShow);
	};
	void DestroyWind() {
		DestroyWindow(_hWnd);
	};

	RECT GetWindowRect() {
		RECT result;
		GetClientRect(_hWnd, &result);

		return result;
	}
	
	HWND GetHWND() { return _hWnd; };
	UINT GetWidth() { return _width; };
	UINT GetHeight() { return _height; };
};

