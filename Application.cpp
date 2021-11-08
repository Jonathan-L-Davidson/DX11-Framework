#include "Application.h"

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




LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

Application::Application()
{
	_hInst = nullptr;
	_hWnd = nullptr;
	_driverType = D3D_DRIVER_TYPE_NULL;
	_featureLevel = D3D_FEATURE_LEVEL_11_0;
	_pd3dDevice = nullptr;
	_pImmediateContext = nullptr;
	_pSwapChain = nullptr;
	_pRenderTargetView = nullptr;
	_pVertexShader = nullptr;
	_pPixelShader = nullptr;
    _pPyramidVertexBuffer = nullptr;
    _pPyramidIndexBuffer = nullptr;
    _pPlaneVertexBuffer = nullptr;
    _pPlaneIndexBuffer = nullptr;
	_pVertexLayout = nullptr;
	_pVertexBuffer = nullptr;
	_pIndexBuffer = nullptr;
	_pConstantBuffer = nullptr;
}

Application::~Application()
{
	Cleanup();
}

HRESULT Application::Initialise(HINSTANCE hInstance, int nCmdShow)
{
    if (FAILED(InitWindow(hInstance, nCmdShow)))
	{
        return E_FAIL;
	}

    RECT rc;
    GetClientRect(_hWnd, &rc);
    _WindowWidth = rc.right - rc.left;
    _WindowHeight = rc.bottom - rc.top;

    if (FAILED(InitDevice()))
    {
        Cleanup();

        return E_FAIL;
    }

	// Initialize the world matrix
	XMStoreFloat4x4(&_world, XMMatrixIdentity());

    // Initialize the view matrix
	XMVECTOR Eye = XMVectorSet(0.0f, 5.0f, -30.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, -3.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMStoreFloat4x4(&_view, XMMatrixLookAtLH(Eye, At, Up));

    // Initialize the projection matrix
	XMStoreFloat4x4(&_projection, XMMatrixPerspectiveFovLH(XM_PIDIV2, _WindowWidth / (FLOAT) _WindowHeight, 0.01f, 100.0f));

	return S_OK;
}

HRESULT Application::InitShadersAndInputLayout()
{
	HRESULT hr;

    // Compile the vertex shader
    ID3DBlob* pVSBlob = nullptr;

    hr = CompileShaderFromFile(L"DX11 Framework.fx", "VS", "vs_4_0", &pVSBlob);


    if (FAILED(hr))
    {

        MessageBox(nullptr,
                   L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        
        return hr;
    }

	// Create the vertex shader
	hr = _pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &_pVertexShader);

	if (FAILED(hr))
	{	
		pVSBlob->Release();
        return hr;
	}

	// Compile the pixel shader
	ID3DBlob* pPSBlob = nullptr;
    hr = CompileShaderFromFile(L"DX11 Framework.fx", "PS", "ps_4_0", &pPSBlob);

    if (FAILED(hr))
    {
        MessageBox(nullptr,
                   L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return hr;
    }

	// Create the pixel shader
	hr = _pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &_pPixelShader);
	pPSBlob->Release();

    if (FAILED(hr))
        return hr;

    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT numElements = ARRAYSIZE(layout);

    // Create the input layout
	hr = _pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
                                        pVSBlob->GetBufferSize(), &_pVertexLayout);
	pVSBlob->Release();

	if (FAILED(hr))
        return hr;

    // Set the input layout
    _pImmediateContext->IASetInputLayout(_pVertexLayout);

	return hr;
}

HRESULT Application::InitVertexBuffer()
{
	HRESULT hr;

    // Create vertex buffer
    SimpleVertex vertices[] =
    {
        { XMFLOAT3( -1.0f, 1.0f, 1.0f ),    XMFLOAT3(-0.816497f,0.408248f, 0.408248),       XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, 1.0f ),     XMFLOAT3(0.816497f, 0.408248f, 0.408248f),      XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f ) },
        { XMFLOAT3( -1.0f, -1.0f, 1.0f ),   XMFLOAT3(-0.666667f, -0.666667f, 0.333333f),    XMFLOAT4( 0.0f, 0.0f, 1.0f, 1.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, 1.0f ),    XMFLOAT3(0.408248f, -0.408248f, 0.816497f),     XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, -1.0f ),   XMFLOAT3(0.666667f, -0.666667f, -0.333333f),    XMFLOAT4( 1.0f, 1.0f, 0.0f, 1.0f ) },
        { XMFLOAT3( -1.0f, -1.0f, -1.0f ),  XMFLOAT3(-0.408248f, -0.408248f,-0.816497f),    XMFLOAT4( 0.0f, 1.0f, 1.0f, 1.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, -1.0f ),    XMFLOAT3(0.333333f, 0.666667f, -0.666667f),     XMFLOAT4( 1.0f, 0.0f, 1.0f, 1.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, -1.0f ),   XMFLOAT3(-0.816497f, 0.408248f, -0.408248f),    XMFLOAT4( 0.5f, 0.5f, 0.5f, 1.0f ) },
    };

    SimpleVertex pyramidVertices[] = {
        { XMFLOAT3(1.0f, 0.0f,-1.0f),       XMFLOAT3(0.333333f,0.333333f,-0.666667f),       XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
        { XMFLOAT3(1.0f, 0.0f, 1.0f),       XMFLOAT3(0.816497f,0.333333f,0.408248f),        XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
        { XMFLOAT3(-1.0f, 0.0f, 1.0f),      XMFLOAT3(-0.333333f,0.333333f,0.666667f),       XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
        { XMFLOAT3(-1.0f, 0.0f,-1.0f),      XMFLOAT3(-0.408248f,0.333333f,-0.816497f),      XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
        { XMFLOAT3(0.0f, 1.0f, 0.0f),       XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
    };

    SimpleVertex planeVertices[] = {
        { XMFLOAT3(-2.0f, 0.0f, 2.0f),      XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT4(0.25f, 0.85f, 0.85f, 1.0f)}, // 0
        { XMFLOAT3(-1.0f, 0.0f, 2.0f),      XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT4(0.25f, 0.85f, 0.85f, 1.0f)}, // 1
        { XMFLOAT3( 0.0f, 0.0f, 2.0f),      XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT4(0.25f, 0.85f, 0.85f, 1.0f)}, // 2
        { XMFLOAT3( 1.0f, 0.0f, 2.0f),      XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT4(0.25f, 0.85f, 0.85f, 1.0f)}, // 3
        { XMFLOAT3( 2.0f, 0.0f, 2.0f),      XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT4(0.25f, 0.85f, 0.85f, 1.0f)}, // 4
        { XMFLOAT3(-2.0f, 0.0f, 1.0f),      XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT4(0.25f, 0.85f, 0.85f, 1.0f)}, // 5
        { XMFLOAT3(-1.0f, 0.0f, 1.0f),      XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT4(0.25f, 0.85f, 0.85f, 1.0f)}, // 6
        { XMFLOAT3( 0.0f, 0.0f, 1.0f),      XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT4(0.25f, 0.85f, 0.85f, 1.0f)}, // 7
        { XMFLOAT3( 1.0f, 0.0f, 1.0f),      XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT4(0.25f, 0.85f, 0.85f, 1.0f)}, // 8
        { XMFLOAT3( 2.0f, 0.0f, 1.0f),      XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT4(0.25f, 0.85f, 0.85f, 1.0f)}, // 9
        { XMFLOAT3(-2.0f, 0.0f, 0.0f),      XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT4(0.25f, 0.85f, 0.85f, 1.0f)}, // 10
        { XMFLOAT3(-1.0f, 0.0f, 0.0f),      XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT4(0.25f, 0.85f, 0.85f, 1.0f)}, // 11
        { XMFLOAT3( 0.0f, 0.0f, 0.0f),      XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT4(0.25f, 0.85f, 0.85f, 1.0f)}, // 12
        { XMFLOAT3( 1.0f, 0.0f, 0.0f),      XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT4(0.25f, 0.85f, 0.85f, 1.0f)}, // 13
        { XMFLOAT3( 2.0f, 0.0f, 0.0f),      XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT4(0.25f, 0.85f, 0.85f, 1.0f)}, // 14
        { XMFLOAT3(-2.0f, 0.0f, -1.0f),     XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT4(0.25f, 0.85f, 0.85f, 1.0f)}, // 15
        { XMFLOAT3(-1.0f, 0.0f, -1.0f),     XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT4(0.25f, 0.85f, 0.85f, 1.0f)}, // 16
        { XMFLOAT3( 0.0f, 0.0f, -1.0f),     XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT4(0.25f, 0.85f, 0.85f, 1.0f)}, // 17
        { XMFLOAT3( 1.0f, 0.0f, -1.0f),     XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT4(0.25f, 0.85f, 0.85f, 1.0f)}, // 18
        { XMFLOAT3( 2.0f, 0.0f, -1.0f),     XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT4(0.25f, 0.85f, 0.85f, 1.0f)}, // 19
        { XMFLOAT3(-2.0f, 0.0f, -2.0f),     XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT4(0.25f, 0.85f, 0.85f, 1.0f)}, // 20
        { XMFLOAT3(-1.0f, 0.0f, -2.0f),     XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT4(0.25f, 0.85f, 0.85f, 1.0f)}, // 21
        { XMFLOAT3( 0.0f, 0.0f, -2.0f),     XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT4(0.25f, 0.85f, 0.85f, 1.0f)}, // 22
        { XMFLOAT3( 1.0f, 0.0f, -2.0f),     XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT4(0.25f, 0.85f, 0.85f, 1.0f)}, // 23
        { XMFLOAT3( 2.0f, 0.0f, -2.0f),     XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT4(0.25f, 0.85f, 0.85f, 1.0f)}, // 24
    };


    // CUBES

    D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex) * 8;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = vertices;

    hr = _pd3dDevice->CreateBuffer(&bd, &InitData, &_pVertexBuffer);

    if (FAILED(hr))
        return hr;

    // PYRAMIDS

    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex) * 5;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = pyramidVertices;

    hr = _pd3dDevice->CreateBuffer(&bd, &InitData, &_pPyramidVertexBuffer);

    if (FAILED(hr))
        return hr;

    // PLANES

    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex) * 25;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = planeVertices;

    hr = _pd3dDevice->CreateBuffer(&bd, &InitData, &_pPlaneVertexBuffer);

    if (FAILED(hr))
        return hr;

	return S_OK;
}

HRESULT Application::InitIndexBuffer()
{
	HRESULT hr;

    // Create index buffer
    WORD indices[] =
    {
        2,1,0, // 3
        3,1,2, // 6
        3,2,4, // 9
        2,5,4, // 12
        2,0,7, // 15
        5,2,7, // 18
        6,4,5, // 21
        5,7,6, // 24
        1,3,4, // 27
        4,6,1, // 30
        1,7,0, // 33
        6,7,1, // 36
    };

    WORD indicesPyramid[] =
    {
        0,1,2, // 3
        0,2,3, // 6
        1,4,2, // 9
        2,4,3, // 12
        0,3,4, // 15
        4,1,0, // 18
    };
    
    WORD indicesPlane[] =
    {
        0,1,5,      // 3
        6,5,1,      // 6
        1,2,6,      // 9
        7,6,2,      // 12
        2,3,7,      // 15
        3,8,7,      // 18
        3,4,8,      // 21
        9,8,4,      // 24
        
        5,6,10,     // 27
        11,10,6,    // 30
        6,7,11,     // 33
        12,11,7,    // 36
        7,8,12,     // 39
        13,12,8,    // 42
        8,9,13,     // 45
        14,13,9,    // 48

        10,11,15,   // 51
        16,15,11,   // 54
        11,12,16,   // 57
        17,16,12,   // 60
        12,13,17,   // 63
        18,17,13,   // 66
        13,14,18,   // 69
        19,18,14,   // 72

        15,16,20,   // 75
        21,20,16,   // 78
        16,17,21,   // 81
        22,21,17,   // 84
        17,18,22,   // 87
        23,22,18,   // 90
        18,19,23,   // 93
        24,23,19,   // 96
    };



	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * sizeof(indices);     
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = indices;
    hr = _pd3dDevice->CreateBuffer(&bd, &InitData, &_pIndexBuffer);

    if (FAILED(hr))
        return hr;

    // Pyramid
    
    ZeroMemory(&bd, sizeof(bd));
    ZeroMemory(&InitData, sizeof(InitData));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * sizeof(indicesPyramid);
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;

    InitData.pSysMem = indicesPyramid;

    hr = _pd3dDevice->CreateBuffer(&bd, &InitData, &_pPyramidIndexBuffer);

    if (FAILED(hr))
        return hr;

    // Plane

    ZeroMemory(&bd, sizeof(bd));
    ZeroMemory(&InitData, sizeof(InitData));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * sizeof(indicesPlane);
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;

    InitData.pSysMem = indicesPlane;

    hr = _pd3dDevice->CreateBuffer(&bd, &InitData, &_pPlaneIndexBuffer);

    if (FAILED(hr))
        return hr;

	return S_OK;
}

HRESULT Application::InitWindow(HINSTANCE hInstance, int nCmdShow)
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_TUTORIAL1);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW );
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"TutorialWindowClass";
    wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);
    if (!RegisterClassEx(&wcex))
        return E_FAIL;

    // Create window
    _hInst = hInstance;
    RECT rc = {0, 0, 640, 480};
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    _hWnd = CreateWindow(L"TutorialWindowClass", L"DX11 Framework", WS_OVERLAPPEDWINDOW,
                         CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
                         nullptr);
    if (!_hWnd)
		return E_FAIL;

    ShowWindow(_hWnd, nCmdShow);

    return S_OK;
}

HRESULT Application::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob;
    hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel, 
        dwShaderFlags, 0, ppBlobOut, &pErrorBlob);

    if (FAILED(hr))
    {
        if (pErrorBlob != nullptr)
            OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());

        if (pErrorBlob) pErrorBlob->Release();

        return hr;
    }

    if (pErrorBlob) pErrorBlob->Release();

    return S_OK;
}

HRESULT Application::InitDevice()
{
    HRESULT hr = S_OK;

    UINT createDeviceFlags = 0;

#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };

    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };

	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = _WindowWidth;
    sd.BufferDesc.Height = _WindowHeight;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = _hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
        _driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain(nullptr, _driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
                                           D3D11_SDK_VERSION, &sd, &_pSwapChain, &_pd3dDevice, &_featureLevel, &_pImmediateContext);
        if (SUCCEEDED(hr))
            break;
    }

    if (FAILED(hr))
        return hr;

    // Create a render target view
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    if (FAILED(hr))
        return hr;

    hr = _pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &_pRenderTargetView);
    pBackBuffer->Release();

    if (FAILED(hr))
        return hr;


    // Setup Depth Buffer
    D3D11_TEXTURE2D_DESC depthStencilDesc; // Descriptor is the way we create Texture2Ds in Dx11

    depthStencilDesc.Width = _WindowWidth;
    depthStencilDesc.Height = _WindowHeight;

    depthStencilDesc.MipLevels = 1; // TODO: Check if this is mipmapping or not and is on basic level.
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    _pd3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, &_pDepthStencilBuffer);
    if (_pDepthStencilBuffer) {
        _pd3dDevice->CreateDepthStencilView(_pDepthStencilBuffer, nullptr, &_pDepthStencilView);
    }

    _pImmediateContext->OMSetRenderTargets(1, &_pRenderTargetView, _pDepthStencilView);

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)_WindowWidth;
    vp.Height = (FLOAT)_WindowHeight;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    _pImmediateContext->RSSetViewports(1, &vp);

	InitShadersAndInputLayout();

	InitVertexBuffer();

	InitIndexBuffer();

    // Set primitive topology
    _pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create the constant buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
    hr = _pd3dDevice->CreateBuffer(&bd, nullptr, &_pConstantBuffer);

    D3D11_RASTERIZER_DESC wfdesc;
    ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
    wfdesc.FillMode = D3D11_FILL_WIREFRAME;
    wfdesc.CullMode = D3D11_CULL_NONE;
    hr = _pd3dDevice->CreateRasterizerState(&wfdesc, &_wireFrame);
    
    if (FAILED(hr))
        return hr;

    return S_OK;
}

void Application::Cleanup()
{
    if (_pImmediateContext) _pImmediateContext->ClearState();

    if (_pConstantBuffer) _pConstantBuffer->Release();
    if (_pVertexBuffer) _pVertexBuffer->Release();
    if (_pIndexBuffer) _pIndexBuffer->Release();
    if (_pVertexLayout) _pVertexLayout->Release();
    if (_pPyramidVertexBuffer) _pPyramidVertexBuffer->Release();
    if (_pPyramidIndexBuffer) _pPyramidIndexBuffer->Release();
    if (_pPlaneVertexBuffer) _pPlaneVertexBuffer->Release();
    if (_pPlaneIndexBuffer) _pPlaneIndexBuffer->Release();
    if (_pVertexShader) _pVertexShader->Release();
    if (_pPixelShader) _pPixelShader->Release();
    if (_pDepthStencilBuffer) _pDepthStencilBuffer->Release();
    if (_pDepthStencilView) _pDepthStencilView->Release();

    if (_pRenderTargetView) _pRenderTargetView->Release();
    if (_pSwapChain) _pSwapChain->Release();
    if (_pImmediateContext) _pImmediateContext->Release();
    if (_pd3dDevice) _pd3dDevice->Release();
    if (_wireFrame) _wireFrame->Release();
}

void Application::Update()
{
    // Update our time
    static float t = 0.0f;

    if (_driverType == D3D_DRIVER_TYPE_REFERENCE)
    {
        t += (float) XM_PI * 0.0125f;
    }
    else
    {
        static DWORD dwTimeStart = 0;
        DWORD dwTimeCur = GetTickCount64();

        if (dwTimeStart == 0)
            dwTimeStart = dwTimeCur;

        t = (dwTimeCur - dwTimeStart) / 1000.0f;
    }

    //
    // Animate the cube
    //
	XMStoreFloat4x4(&_world, XMMatrixScaling(2.5f, 2.5f, 2.5f) * XMMatrixRotationY(t * 0.2f));

	XMStoreFloat4x4(&_world2, XMMatrixScaling(1.4f, 1.4f, 1.4f) * XMMatrixRotationZ(t)
        * XMMatrixRotationX(t) * XMMatrixTranslation(30.0f,0.0f,0.0f) * XMMatrixRotationY(t * 0.4));
	
    XMStoreFloat4x4(&_world3, XMMatrixScaling(1.5f, 1.5f, 1.5f) * XMMatrixRotationY(t)
        * XMMatrixRotationX(50) * XMMatrixTranslation(24.0f,0,0) * XMMatrixRotationY(t * 0.6));

    XMStoreFloat4x4(&_world4, XMMatrixScaling(1.3f, 1.3f, 1.3f) * XMMatrixRotationY(t)
        * XMMatrixRotationX(50) * XMMatrixTranslation(12.0f, 0, 0) * XMMatrixRotationY(t * 0.7));

    XMStoreFloat4x4(&_world5, XMMatrixScaling(0.5f, 0.5f, 0.5f) * XMMatrixRotationY(t * 2) * XMMatrixRotationX(-50)
        * XMMatrixTranslation(24.0f, 0, 0) * XMMatrixRotationY(t * 0.6) * XMMatrixTranslation(-3.0f, 0, 0));

    XMStoreFloat4x4(&_world6, XMMatrixScaling(0.5f, 0.5f, 0.5f) * XMMatrixRotationY(t * 2) * XMMatrixRotationX(50) 
        * XMMatrixTranslation(12.0f, 0, 0) * XMMatrixRotationY(t * 0.7) * XMMatrixTranslation(3.0f, 0, 0));

    XMStoreFloat4x4(&_world7, XMMatrixScaling(10, 1.0f, 10) * XMMatrixTranslation(0, -5.0f, 0));
}

void Application::Draw()
{
    //
    // Clear the back buffer
    //
    float ClearColor[4] = {0.05f, 0.05f, 0.05f, 1.0f}; // red,green,blue,alpha
    _pImmediateContext->ClearRenderTargetView(_pRenderTargetView, ClearColor);
    _pImmediateContext->ClearDepthStencilView(_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	XMMATRIX world = XMLoadFloat4x4(&_world);
	XMMATRIX view = XMLoadFloat4x4(&_view);
	XMMATRIX projection = XMLoadFloat4x4(&_projection);
    
    //
    // Update variables
    //
    
    // TODO: Make a "get time" or make it a global variable.
    static float t = 0.0f;

    if (_driverType == D3D_DRIVER_TYPE_REFERENCE)
    {
        t += (float)XM_PI * 0.0125f;
    }
    else
    {
        static DWORD dwTimeStart = 0;
        DWORD dwTimeCur = GetTickCount64();

        if (dwTimeStart == 0)
            dwTimeStart = dwTimeCur;

        t = (dwTimeCur - dwTimeStart) / 1000.0f;
    }

    ConstantBuffer cb;
	cb.mWorld = XMMatrixTranspose(world);
	cb.mView = XMMatrixTranspose(view);
	cb.mProjection = XMMatrixTranspose(projection);
    cb.mTime = t;
    cb.mLightDirection = XMFLOAT3(0.25f, 0.5f, 1.0f);
    cb.mDiffuseMaterial = XMFLOAT4(0.8f, 0.5f, 0.5f, 1.0f);
    cb.mDiffuseLight = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);


    // Set vertex buffer
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;
    _pImmediateContext->IASetVertexBuffers(0, 1, &_pPyramidVertexBuffer, &stride, &offset);

    // Set index buffer
    _pImmediateContext->IASetIndexBuffer(_pPyramidIndexBuffer, DXGI_FORMAT_R16_UINT, 0);


	_pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);

    if (GetKeyState(0x52) & 0x8000 && !_lastInput) {
        _wireFrameToggle = _wireFrameToggle ? false : true;

        _lastInput = 1000;
        
    }

    if (_wireFrameToggle) {
        _pImmediateContext->RSSetState(_wireFrame);
    }
    else {
        _pImmediateContext->RSSetState(nullptr);
    }

    //
    // Renders a triangle
    //

    _pImmediateContext->VSSetShader(_pVertexShader, nullptr, 0);
	_pImmediateContext->VSSetConstantBuffers(0, 1, &_pConstantBuffer);
    _pImmediateContext->PSSetConstantBuffers(0, 1, &_pConstantBuffer);
	_pImmediateContext->PSSetShader(_pPixelShader, nullptr, 0);
	_pImmediateContext->DrawIndexed(18, 0, 0);        

    // Switch to rendering the cube
    _pImmediateContext->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);
    _pImmediateContext->IASetIndexBuffer(_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

    world = XMLoadFloat4x4(&_world2);
    cb.mWorld = XMMatrixTranspose(world);
    _pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
    _pImmediateContext->DrawIndexed(36, 0, 0);

    world = XMLoadFloat4x4(&_world3);
    cb.mWorld = XMMatrixTranspose(world);
    _pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
    _pImmediateContext->DrawIndexed(36, 0, 0);

    world = XMLoadFloat4x4(&_world4);
    cb.mWorld = XMMatrixTranspose(world);
    _pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
    _pImmediateContext->DrawIndexed(36, 0, 0);


    // Switch to rendering the pyramid
    _pImmediateContext->IASetVertexBuffers(0, 1, &_pPyramidVertexBuffer, &stride, &offset);
    _pImmediateContext->IASetIndexBuffer(_pPyramidIndexBuffer, DXGI_FORMAT_R16_UINT, 0);


    world = XMLoadFloat4x4(&_world5);
    cb.mWorld = XMMatrixTranspose(world);
    _pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
    _pImmediateContext->DrawIndexed(18, 0, 0);

    world = XMLoadFloat4x4(&_world6);
    cb.mWorld = XMMatrixTranspose(world);
    _pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
    _pImmediateContext->DrawIndexed(18, 0, 0);

    // Switch to rendering the plane.
    _pImmediateContext->IASetVertexBuffers(0, 1, &_pPlaneVertexBuffer, &stride, &offset);
    _pImmediateContext->IASetIndexBuffer(_pPlaneIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

    world = XMLoadFloat4x4(&_world7);
    cb.mWorld = XMMatrixTranspose(world);
    _pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
    _pImmediateContext->DrawIndexed(96, 0, 0);


    //
    // Present our back buffer to our front buffer
    //
    _pSwapChain->Present(0, 0);

    if (_lastInput > 0) {
        _lastInput--;
    }
    else {
        _lastInput = 0;
    }
}