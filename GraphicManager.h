#pragma once
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "DxDevice.h"
#include "Window.h"
#include "Structures.h"
#include "ObjectManager.h"

class GraphicManager {
private:
	DxDevice*					_dxDevice;
	Window*						_window;

	ID3D11Buffer*				_constantBuffer;
	IDXGISwapChain*				_swapChain;
	ID3D11DepthStencilView*		_depthStencilView;
	ID3D11Texture2D*			_depthStencilBuffer;
	ID3D11RenderTargetView*		_renderTargetView;
	ID3D11RasterizerState*		_rasterState;

	ID3D11Device*				_d3dDevice;			// Pointer gotten from DxDevice
	ID3D11DeviceContext*		_immediateContext;	// Pointer gotten from DxDevice

	XMFLOAT4X4					_world;

private:
	void Draw();

public:
	GraphicManager();
	~GraphicManager();

	void Destroy();
	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow, LPCWSTR windowName, LPCWSTR windowClass);

	void Update();

public:
	ID3D11Buffer*			GetCB()				{ return _constantBuffer; }; // Get ConstantBuffer
	IDXGISwapChain*			GetSwapChain()		{ return _swapChain; };
	ID3D11DepthStencilView* GetDStencilView()	{ return _depthStencilView; };
	ID3D11Texture2D*		GetDStencilBuffer()	{ return _depthStencilBuffer; };
	ID3D11RenderTargetView* GetRTV()			{ return _renderTargetView; }; // Get RenderTargetView
	ID3D11RasterizerState*	GetRasterState()	{ return _rasterState; };
	DxDevice*				GetDevice()			{ return _dxDevice; };
	Window*					GetWindow()			{ return _window; };

};

