#pragma once
#include <d3d11_1.h>
#include <DirectXMath.h>
#include "resource.h"
#include "Camera.h"

class Window {
private:
	HINSTANCE	_hInst;
	HWND		_hWnd;
	
	int			_cmdShow;
	LPCWSTR		_windowName;
	LPCWSTR		_windowClass;

	RECT		_size = { 0, 0, 1920, 1200 };

	UINT		_height;
	UINT		_width;

	Camera*		_camera;

	XMFLOAT4X4	_view;
	XMFLOAT4X4	_projection;

public:
	Window(HINSTANCE hInstance, int nCmdShow, LPCWSTR windowName, LPCWSTR windowClass);
	~Window();

	HRESULT Initialise();
	void Destroy();

	void UpdateViewMatrix();

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
	};
	
	HWND GetHWND() { return _hWnd; };
	UINT GetWidth() { return _width; };
	UINT GetHeight() { return _height; };

	XMMATRIX GetViewMatrix() { return XMLoadFloat4x4(&_view); };
	XMMATRIX GetProjectionMatrix() { return XMLoadFloat4x4(&_projection); };
};

