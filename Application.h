#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "resource.h"

using namespace DirectX;

struct SimpleVertex
{
    XMFLOAT3 Pos;
    XMFLOAT4 Color;
	XMFLOAT3 Normal;

};

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
	float mTime;

	XMFLOAT3 mLightDirection;
	XMFLOAT4 mDiffuseMaterial;
	XMFLOAT4 mDiffuseLight;

	XMFLOAT4 mAmbientMaterial;
	XMFLOAT4 mAmbientLight;
};

class Application
{
private:
	HINSTANCE               _hInst;
	HWND                    _hWnd;
	D3D_DRIVER_TYPE         _driverType;
	D3D_FEATURE_LEVEL       _featureLevel;
	ID3D11Device*           _pd3dDevice;
	ID3D11DeviceContext*    _pImmediateContext;
	IDXGISwapChain*         _pSwapChain;
	ID3D11DepthStencilView* _pDepthStencilView;
	ID3D11Texture2D*		_pDepthStencilBuffer;
	ID3D11RenderTargetView* _pRenderTargetView;
	ID3D11VertexShader*     _pVertexShader;
	ID3D11PixelShader*      _pPixelShader;
	ID3D11InputLayout*      _pVertexLayout;
	ID3D11Buffer*           _pVertexBuffer;
	ID3D11Buffer*           _pIndexBuffer;
	ID3D11Buffer*           _pPyramidVertexBuffer;
	ID3D11Buffer*           _pPyramidIndexBuffer;
	ID3D11Buffer*			_pPlaneVertexBuffer;
	ID3D11Buffer*           _pPlaneIndexBuffer;
	ID3D11Buffer*           _pConstantBuffer;
	XMFLOAT4X4              _world, _world2, _world3,
							_world4, _world5, _world6,
							_world7;
	XMFLOAT4X4              _view;
	XMFLOAT4X4              _projection;

	ID3D11RasterizerState*	_wireFrame;
	bool					_wireFrameToggle = false;
	UINT					_lastInput = 0;

private:
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	HRESULT InitDevice();
	void Cleanup();
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT InitShadersAndInputLayout();
	HRESULT InitVertexBuffer();
	HRESULT InitIndexBuffer();

	UINT _WindowHeight;
	UINT _WindowWidth;

public:
	Application();
	~Application();

	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);

	void Update();
	void Draw();
};
