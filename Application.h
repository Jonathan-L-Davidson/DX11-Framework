#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "resource.h"
#include "TextureLoader.h"
#include "OBJLoader.h"
#include "Structures.h"
#include "Window.h"

using namespace DirectX;


class Application {
private:
	HINSTANCE               _hInst;

	Window*					_window;

	IDXGISwapChain*         _pSwapChain;
	ID3D11DepthStencilView* _pDepthStencilView;
	ID3D11Texture2D*		_pDepthStencilBuffer;
	ID3D11RenderTargetView* _pRenderTargetView;
	ID3D11VertexShader*     _pVertexShader;
	ID3D11PixelShader*      _pPixelShader;
	ID3D11InputLayout*      _pVertexLayout;

	ID3D11ShaderResourceView* _pCrateTextureRV;
	ID3D11ShaderResourceView* _pCrateSpecTextureRV;
	ID3D11ShaderResourceView* _pCrateNormTextureRV;
	ID3D11ShaderResourceView* _pHerculesTextureRV;
	ID3D11SamplerState* _pSamplerLinear;

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

	MeshData _objMeshData;
private:
	HRESULT InitDevice();
	void Cleanup();
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT InitShadersAndInputLayout();
	HRESULT InitVertexBuffer();
	HRESULT InitIndexBuffer();

public:
	Application();
	~Application();

	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);

	void Update();
	void Draw();
};

