#include "Application.h"

#define WINDOW_NAME L"DX11 Framework"
#define WINDOW_CLASS L"DX11 Framework"

// New datatypes/procs I need to learn:
// HDC - Handler for Device Context
// PAINTSTRUCT - contains app info to "paint" the client area?? - NEED MORE INFO
// 
// 
// DefWindowProc() - Default handler for window proc.
// 
// 
// Check notes for all other things, it's just like Dx12
// Just without GPU synchronisation, makes life easier.
// 
// SwapChain is NOT SwapChain1. Swapchain1+ has extra features
// like dirty rects (which keeps segments if there hasnt been any
// changes to it.)
// 
// 
// 
//  
//

Application::Application()
{
	_hInst = nullptr;
    _graphicManager = nullptr;
    _objectManager = nullptr;
    _textureManager = nullptr;

    _cube = nullptr;
}

Application::~Application()
{
	Cleanup();
}

HRESULT Application::Initialise(HINSTANCE hInstance, int nCmdShow) {

    _graphicManager = new GraphicManager();
    
    if (FAILED(_graphicManager->Initialise(hInstance, nCmdShow, WINDOW_NAME, WINDOW_CLASS))) {
        _graphicManager->Destroy();
        return E_FAIL;
	}

    _textureManager = new TextureManager();
    _textureManager->SetDevice(_graphicManager->GetDevice());
    _textureManager->Initialise();
    
    _objectManager = new ObjectManager();
    _objectManager->Initialise();

    _graphicManager->SetObjectManager(_objectManager);

    _cube = new Object();
    MeshData* model = new MeshData(OBJLoader::Load("Hercules.obj", _graphicManager->GetDevice()->GetDevice()));

    _cube->LoadModel(model);
    //LPCWSTR colorTexture = L"Hercules_COLOR";
    //_cube->LoadTexture(_textureManager->GetTexture(colorTexture));
    Texture* texture = new Texture(L"Pain");
    _cube->LoadTexture(texture->LoadTexture(L"Hercules_COLOR.dds", _graphicManager->GetDevice()->GetDevice()));

    _objectManager->AddObject(_cube);

	return S_OK;
}

//HRESULT Application::InitVertexBuffer()
//{
//	HRESULT hr;
//
//    // Create vertex buffer
//
//    SimpleVertex vertices[] =
//    {
//        { XMFLOAT3( -1.0f, 1.0f, 1.0f ),    XMFLOAT3(-0.816497f,0.408248f, 0.408248),       XMFLOAT2(0.0f, 0.0f), },
//        { XMFLOAT3( 1.0f, 1.0f, 1.0f ),     XMFLOAT3(0.816497f, 0.408248f, 0.408248f),      XMFLOAT2(1.0f, 0.0f), },
//        { XMFLOAT3( -1.0f, -1.0f, 1.0f ),   XMFLOAT3(-0.666667f, -0.666667f, 0.333333f),    XMFLOAT2(0.0f, 1.0f), },
//        { XMFLOAT3( 1.0f, -1.0f, 1.0f ),    XMFLOAT3(0.408248f, -0.408248f, 0.816497f),     XMFLOAT2(1.0f, 1.0f), },
//        { XMFLOAT3( 1.0f, -1.0f, -1.0f ),   XMFLOAT3(0.666667f, -0.666667f, -0.333333f),    XMFLOAT2(1.0f, 1.0f), },
//        { XMFLOAT3( -1.0f, -1.0f, -1.0f ),  XMFLOAT3(-0.408248f, -0.408248f,-0.816497f),    XMFLOAT2(0.0f, 1.0f), },
//        { XMFLOAT3( 1.0f, 1.0f, -1.0f ),    XMFLOAT3(0.333333f, 0.666667f, -0.666667f),     XMFLOAT2(1.0f, 0.0f), },
//        { XMFLOAT3( -1.0f, 1.0f, -1.0f ),   XMFLOAT3(-0.816497f, 0.408248f, -0.408248f),    XMFLOAT2(0.0f, 0.0f), },
//    };
//
//    SimpleVertex pyramidVertices[] = {
//        { XMFLOAT3(1.0f, 0.0f,-1.0f),       XMFLOAT3(0.333333f,0.333333f,-0.666667f),       XMFLOAT2(1.0f, 1.0f), },
//        { XMFLOAT3(1.0f, 0.0f, 1.0f),       XMFLOAT3(0.816497f,0.333333f,0.408248f),        XMFLOAT2(0.0f, 1.0f), },
//        { XMFLOAT3(-1.0f, 0.0f, 1.0f),      XMFLOAT3(-0.333333f,0.333333f,0.666667f),       XMFLOAT2(1.0f, 1.0f), },
//        { XMFLOAT3(-1.0f, 0.0f,-1.0f),      XMFLOAT3(-0.408248f,0.333333f,-0.816497f),      XMFLOAT2(0.0f, 1.0f), },
//        { XMFLOAT3(0.0f, 1.0f, 0.0f),       XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT2(0.5f, 0.0f), },
//    };
//
//    SimpleVertex planeVertices[] = {
//        { XMFLOAT3(-2.0f, 0.0f, 2.0f),      XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT2(0.0f,  0.0f),}, // 0
//        { XMFLOAT3(-1.0f, 0.0f, 2.0f),      XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT2(0.25f, 0.0f),}, // 1
//        { XMFLOAT3(0.0f, 0.0f, 2.0f),       XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT2(0.50f, 0.0f),}, // 2
//        { XMFLOAT3(1.0f, 0.0f, 2.0f),       XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT2(0.75f, 0.0f),}, // 3
//        { XMFLOAT3(2.0f, 0.0f, 2.0f),       XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT2(1.0f,  0.0f),}, // 4
//        { XMFLOAT3(-2.0f, 0.0f, 1.0f),      XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT2(0.0f, 0.25f),}, // 5
//        { XMFLOAT3(-1.0f, 0.0f, 1.0f),      XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT2(0.25f,0.25f),}, // 6
//        { XMFLOAT3(0.0f, 0.0f, 1.0f),       XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT2(0.50f,0.25f),}, // 7
//        { XMFLOAT3(1.0f, 0.0f, 1.0f),       XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT2(0.75f,0.25f),}, // 8
//        { XMFLOAT3(2.0f, 0.0f, 1.0f),       XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT2(1.0f, 0.25f),}, // 9
//        { XMFLOAT3(-2.0f, 0.0f, 0.0f),      XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT2(0.0f, 0.50f),}, // 10
//        { XMFLOAT3(-1.0f, 0.0f, 0.0f),      XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT2(0.25f,0.50f),}, // 11
//        { XMFLOAT3(0.0f, 0.0f, 0.0f),       XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT2(0.50f,0.50f),}, // 12
//        { XMFLOAT3(1.0f, 0.0f, 0.0f),       XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT2(0.75f,0.50f),}, // 13
//        { XMFLOAT3(2.0f, 0.0f, 0.0f),       XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT2(1.0f, 0.50f),}, // 14
//        { XMFLOAT3(-2.0f, 0.0f, -1.0f),     XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT2(0.0f, 0.75f),}, // 15
//        { XMFLOAT3(-1.0f, 0.0f, -1.0f),     XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT2(0.25f,0.75f),}, // 16
//        { XMFLOAT3(0.0f, 0.0f, -1.0f),      XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT2(0.50f,0.75f),}, // 17
//        { XMFLOAT3(1.0f, 0.0f, -1.0f),      XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT2(0.75f,0.75f),}, // 18
//        { XMFLOAT3(2.0f, 0.0f, -1.0f),      XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT2(1.0f, 0.75f),}, // 19
//        { XMFLOAT3(-2.0f, 0.0f, -2.0f),     XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT2(0.0f,  1.0f),}, // 20
//        { XMFLOAT3(-1.0f, 0.0f, -2.0f),     XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT2(0.25f, 1.0f),}, // 21
//        { XMFLOAT3(0.0f, 0.0f, -2.0f),      XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT2(0.50f, 1.0f),}, // 22
//        { XMFLOAT3(1.0f, 0.0f, -2.0f),      XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT2(0.75f, 1.0f),}, // 23
//        { XMFLOAT3(2.0f, 0.0f, -2.0f),      XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT2(1.0f,  1.0f),}, // 24
//    };
//
//
//    // CUBES
//
//    D3D11_BUFFER_DESC bd;
//	ZeroMemory(&bd, sizeof(bd));
//    bd.Usage = D3D11_USAGE_DEFAULT;
//    bd.ByteWidth = sizeof(SimpleVertex) * 8;
//    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	bd.CPUAccessFlags = 0;
//
//    D3D11_SUBRESOURCE_DATA InitData;
//	ZeroMemory(&InitData, sizeof(InitData));
//    InitData.pSysMem = vertices;
//
//    hr = _pd3dDevice->CreateBuffer(&bd, &InitData, &_pVertexBuffer);
//
//    if (FAILED(hr))
//        return hr;
//
//    // PYRAMIDS
//
//    ZeroMemory(&bd, sizeof(bd));
//    bd.Usage = D3D11_USAGE_DEFAULT;
//    bd.ByteWidth = sizeof(SimpleVertex) * 5;
//    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//    bd.CPUAccessFlags = 0;
//
//    ZeroMemory(&InitData, sizeof(InitData));
//    InitData.pSysMem = pyramidVertices;
//
//    hr = _pd3dDevice->CreateBuffer(&bd, &InitData, &_pPyramidVertexBuffer);
//
//    if (FAILED(hr))
//        return hr;
//
//    // PLANES
//
//    ZeroMemory(&bd, sizeof(bd));
//    bd.Usage = D3D11_USAGE_DEFAULT;
//    bd.ByteWidth = sizeof(SimpleVertex) * 25;
//    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//    bd.CPUAccessFlags = 0;
//
//    ZeroMemory(&InitData, sizeof(InitData));
//    InitData.pSysMem = planeVertices;
//
//    hr = _pd3dDevice->CreateBuffer(&bd, &InitData, &_pPlaneVertexBuffer);
//
//    if (FAILED(hr))
//        return hr;
//
//	return S_OK;
//}
//
//HRESULT Application::InitIndexBuffer()
//{
//	HRESULT hr;
//
//    // Create index buffer
//    WORD indices[] =
//    {
//        2,1,0, // 3
//        3,1,2, // 6
//        3,2,4, // 9
//        2,5,4, // 12
//        2,0,7, // 15
//        5,2,7, // 18
//        6,4,5, // 21
//        5,7,6, // 24
//        1,3,4, // 27
//        4,6,1, // 30
//        1,7,0, // 33
//        6,7,1, // 36
//    };
//
//    WORD indicesPyramid[] =
//    {
//        0,1,2, // 3
//        0,2,3, // 6
//        1,4,2, // 9
//        2,4,3, // 12
//        0,3,4, // 15
//        4,1,0, // 18
//    };
//    
//    WORD indicesPlane[] =
//    {
//        0,1,5,      // 3
//        6,5,1,      // 6
//        1,2,6,      // 9
//        7,6,2,      // 12
//        2,3,7,      // 15
//        3,8,7,      // 18
//        3,4,8,      // 21
//        9,8,4,      // 24
//        
//        5,6,10,     // 27
//        11,10,6,    // 30
//        6,7,11,     // 33
//        12,11,7,    // 36
//        7,8,12,     // 39
//        13,12,8,    // 42
//        8,9,13,     // 45
//        14,13,9,    // 48
//
//        10,11,15,   // 51
//        16,15,11,   // 54
//        11,12,16,   // 57
//        17,16,12,   // 60
//        12,13,17,   // 63
//        18,17,13,   // 66
//        13,14,18,   // 69
//        19,18,14,   // 72
//
//        15,16,20,   // 75
//        21,20,16,   // 78
//        16,17,21,   // 81
//        22,21,17,   // 84
//        17,18,22,   // 87
//        23,22,18,   // 90
//        18,19,23,   // 93
//        24,23,19,   // 96
//    };
//
//
//
//	D3D11_BUFFER_DESC bd;
//	ZeroMemory(&bd, sizeof(bd));
//
//    bd.Usage = D3D11_USAGE_DEFAULT;
//    bd.ByteWidth = sizeof(WORD) * sizeof(indices);     
//    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	bd.CPUAccessFlags = 0;
//
//	D3D11_SUBRESOURCE_DATA InitData;
//	ZeroMemory(&InitData, sizeof(InitData));
//    InitData.pSysMem = indices;
//    hr = _pd3dDevice->CreateBuffer(&bd, &InitData, &_pIndexBuffer);
//
//    if (FAILED(hr))
//        return hr;
//
//    // Pyramid
//    
//    ZeroMemory(&bd, sizeof(bd));
//    ZeroMemory(&InitData, sizeof(InitData));
//
//    bd.Usage = D3D11_USAGE_DEFAULT;
//    bd.ByteWidth = sizeof(WORD) * sizeof(indicesPyramid);
//    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
//    bd.CPUAccessFlags = 0;
//
//    InitData.pSysMem = indicesPyramid;
//
//    hr = _pd3dDevice->CreateBuffer(&bd, &InitData, &_pPyramidIndexBuffer);
//
//    if (FAILED(hr))
//        return hr;
//
//    // Plane
//
//    ZeroMemory(&bd, sizeof(bd));
//    ZeroMemory(&InitData, sizeof(InitData));
//
//    bd.Usage = D3D11_USAGE_DEFAULT;
//    bd.ByteWidth = sizeof(WORD) * sizeof(indicesPlane);
//    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
//    bd.CPUAccessFlags = 0;
//
//    InitData.pSysMem = indicesPlane;
//
//    hr = _pd3dDevice->CreateBuffer(&bd, &InitData, &_pPlaneIndexBuffer);
//
//    if (FAILED(hr))
//        return hr;
//
//	return S_OK;
//}


void Application::Cleanup()
{
    //if (_pVertexBuffer) _pVertexBuffer->Release();
    //if (_pIndexBuffer) _pIndexBuffer->Release();
    //if (_pPyramidVertexBuffer) _pPyramidVertexBuffer->Release();
    //if (_pPyramidIndexBuffer) _pPyramidIndexBuffer->Release();
    //if (_pPlaneVertexBuffer) _pPlaneVertexBuffer->Release();
    //if (_pPlaneIndexBuffer) _pPlaneIndexBuffer->Release();
}

void Application::Update()
{
    // Update our time
    static float t = 0.0f;

    t += (float) XM_PI * 0.0125f;

 //   //
 //   // Animate the cube
 //   //
	//XMStoreFloat4x4(&_world, XMMatrixScaling(2.5f, 2.5f, 2.5f) * XMMatrixRotationY(t * 0.2f));

	//XMStoreFloat4x4(&_world2, XMMatrixScaling(1.4f, 1.4f, 1.4f) * XMMatrixRotationZ(t)
 //       * XMMatrixRotationX(t) * XMMatrixTranslation(30.0f,0.0f,0.0f) * XMMatrixRotationY(t * 0.4));
	//
 //   XMStoreFloat4x4(&_world3, XMMatrixScaling(1.5f, 1.5f, 1.5f) * XMMatrixRotationY(t)
 //       * XMMatrixRotationX(50) * XMMatrixTranslation(24.0f,0,0) * XMMatrixRotationY(t * 0.6));

 //   XMStoreFloat4x4(&_world4, XMMatrixScaling(1.3f, 1.3f, 1.3f) * XMMatrixRotationY(t)
 //       * XMMatrixRotationX(50) * XMMatrixTranslation(12.0f, 0, 0) * XMMatrixRotationY(t * 0.7));

 //   XMStoreFloat4x4(&_world5, XMMatrixScaling(0.5f, 0.5f, 0.5f) * XMMatrixRotationY(t * 2) * XMMatrixRotationX(-50)
 //       * XMMatrixTranslation(24.0f, 0, 0) * XMMatrixRotationY(t * 0.6) * XMMatrixTranslation(-3.0f, 0, 0));

 //   XMStoreFloat4x4(&_world6, XMMatrixScaling(0.5f, 0.5f, 0.5f) * XMMatrixRotationY(t * 2) * XMMatrixRotationX(50) 
 //       * XMMatrixTranslation(12.0f, 0, 0) * XMMatrixRotationY(t * 0.7) * XMMatrixTranslation(3.0f, 0, 0));
 //   XMStoreFloat4x4(&_world7, XMMatrixScaling(10, 1.0f, 10) * XMMatrixTranslation(0, -5.0f, 0));
}

void Application::Draw() {
    _graphicManager->Update();
}

//void Application::Draw()
//{
//    //
//    // Clear the back buffer
//    //
//    float ClearColor[4] = {0.05f, 0.05f, 0.05f, 1.0f}; // red,green,blue,alpha
//    _pImmediateContext->ClearRenderTargetView(_pRenderTargetView, ClearColor);
//    _pImmediateContext->ClearDepthStencilView(_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
//
//	XMMATRIX world = XMLoadFloat4x4(&_world);
//	XMMATRIX view = XMLoadFloat4x4(&_view);
//	XMMATRIX projection = XMLoadFloat4x4(&_projection);
//    
//    //
//    // Update variables
//    //
//    
//    // TODO: Make a "get time" or make it a global variable.
//    static float t = 0.0f;
//
//    if (_driverType == D3D_DRIVER_TYPE_REFERENCE)
//    {
//        t += (float)XM_PI * 0.0125f;
//    }
//    else
//    {
//        static DWORD dwTimeStart = 0;
//        DWORD dwTimeCur = GetTickCount64();
//
//        if (dwTimeStart == 0)
//            dwTimeStart = dwTimeCur;
//
//        t = (dwTimeCur - dwTimeStart) / 1000.0f;
//    }
//
//    ConstantBuffer cb;
//	  cb.mWorld = XMMatrixTranspose(world);
//	  cb.mView = XMMatrixTranspose(view);
//	  cb.mProjection = XMMatrixTranspose(projection);
//    cb.mTime = t;
//
//    cb.mLightDirection = XMFLOAT3(0.25f, 1.0f, 0.25f);
//
//    cb.mDiffuseLight = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
//    cb.mDiffuseMaterial = XMFLOAT4(0.8f, 0.5f, 0.5f, 1.0f);
//
//    cb.mAmbientLight = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
//    cb.mAmbientMaterial= XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
//    
//    cb.mSpecularLight = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
//    cb.mSpecularMaterial = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
//    cb.mSpecularPower = 10.0f;
//    cb.mEyePosW = XMFLOAT3(0.0f, 5.0f, -30.0f);
//
//
//    // Set vertex buffer
//    UINT stride = sizeof(SimpleVertex);
//    UINT offset = 0;
//    _pImmediateContext->IASetVertexBuffers(0, 1, &_pPyramidVertexBuffer, &stride, &offset);
//
//    // Set index buffer
//    _pImmediateContext->IASetIndexBuffer(_pPyramidIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
//
//
//	_pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
//
//    if (GetKeyState(0x52) & 0x8000 && !_lastInput) {
//        _wireFrameToggle = _wireFrameToggle ? false : true;
//
//        _lastInput = 1000;
//        
//    }
//
//    if (_wireFrameToggle) {
//        _pImmediateContext->RSSetState(_wireFrame);
//    }
//    else {
//        _pImmediateContext->RSSetState(nullptr);
//    }
//
//    //
//    // Renders a triangle
//    //
//
//    _pImmediateContext->VSSetShader(_pVertexShader, nullptr, 0);
//	_pImmediateContext->VSSetConstantBuffers(0, 1, &_pConstantBuffer);
//    _pImmediateContext->PSSetConstantBuffers(0, 1, &_pConstantBuffer);
//	_pImmediateContext->PSSetShader(_pPixelShader, nullptr, 0);
//	
//
//    // Switch to rendering the Hercules.obj
//    _pImmediateContext->IASetVertexBuffers(0, 1, &_objMeshData.VertexBuffer, &_objMeshData.VBStride, &_objMeshData.VBOffset);
//    _pImmediateContext->IASetIndexBuffer(_objMeshData.IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
//    _pImmediateContext->PSSetShaderResources(0, 1, &_pHerculesTextureRV);
//
//    _pImmediateContext->DrawIndexed(_objMeshData.IndexCount, 0, 0);
//    
//    
//    // Switch to rendering the cube
//    _pImmediateContext->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);
//    _pImmediateContext->IASetIndexBuffer(_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
//    _pImmediateContext->PSSetShaderResources(0, 1, &_pCrateTextureRV);
//
//    world = XMLoadFloat4x4(&_world2);
//    cb.mWorld = XMMatrixTranspose(world);
//    _pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
//    _pImmediateContext->DrawIndexed(36, 0, 0);
//
//    world = XMLoadFloat4x4(&_world3);
//    cb.mWorld = XMMatrixTranspose(world);
//    _pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
//    _pImmediateContext->DrawIndexed(36, 0, 0);
//
//    world = XMLoadFloat4x4(&_world4);
//    cb.mWorld = XMMatrixTranspose(world);
//    _pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
//    _pImmediateContext->DrawIndexed(36, 0, 0);
//
//
//    
//    // Switch to rendering the pyramid
//    _pImmediateContext->IASetVertexBuffers(0, 1, &_pPyramidVertexBuffer, &stride, &offset);
//    _pImmediateContext->IASetIndexBuffer(_pPyramidIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
//    _pImmediateContext->PSSetShaderResources(0, 1, &_pCrateTextureRV);
//
//
//    world = XMLoadFloat4x4(&_world5);
//    cb.mWorld = XMMatrixTranspose(world);
//    _pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
//    _pImmediateContext->DrawIndexed(18, 0, 0);
//
//    world = XMLoadFloat4x4(&_world6);
//    cb.mWorld = XMMatrixTranspose(world);
//    _pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
//    _pImmediateContext->DrawIndexed(18, 0, 0);
//
//    // Switch to rendering the plane.
//    _pImmediateContext->IASetVertexBuffers(0, 1, &_pPlaneVertexBuffer, &stride, &offset);
//    _pImmediateContext->IASetIndexBuffer(_pPlaneIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
//
//    world = XMLoadFloat4x4(&_world7);
//    cb.mWorld = XMMatrixTranspose(world);
//    _pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
//    _pImmediateContext->DrawIndexed(96, 0, 0);
//
//
//    //
//    // Present our back buffer to our front buffer
//    //
//    _pSwapChain->Present(0, 0);
//
//    if (_lastInput > 0) {
//        _lastInput--;
//    }
//    else {
//        _lastInput = 0;
//    }
//}