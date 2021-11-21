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
#include "GraphicManager.h"

using namespace DirectX;


class Application {
private:
	HINSTANCE               _hInst;
	GraphicManager*			_graphicManager;


	ID3D11Buffer*           _pVertexBuffer;
	ID3D11Buffer*           _pIndexBuffer;
	ID3D11Buffer*           _pPyramidVertexBuffer;
	ID3D11Buffer*           _pPyramidIndexBuffer;
	ID3D11Buffer*			_pPlaneVertexBuffer;
	ID3D11Buffer*           _pPlaneIndexBuffer;
	XMFLOAT4X4              _world, _world2, _world3,
							_world4, _world5, _world6,
							_world7;
	XMFLOAT4X4              _view;
	XMFLOAT4X4              _projection;

	bool					_wireFrameToggle = false;
	UINT					_lastInput = 0;

	MeshData _objMeshData;
private:
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

