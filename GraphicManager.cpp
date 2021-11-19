#include "GraphicManager.h"

GraphicManager::GraphicManager() {
	_dxDevice = nullptr;
	_window = nullptr;
	_swapChain = nullptr;
	_renderTargetView = nullptr;
	_constantBuffer = nullptr;

}

GraphicManager::~GraphicManager() {
	Destroy();
}

void GraphicManager::Destroy() {
    if (_constantBuffer) _constantBuffer->Release();
    if (_depthStencilBuffer) _depthStencilBuffer->Release();
    if (_depthStencilView) _depthStencilView->Release();

    if (_renderTargetView) _renderTargetView->Release();
    if (_swapChain) _swapChain->Release();
    if (_rasterState) _rasterState->Release();
}


HRESULT GraphicManager::Initialise(HINSTANCE hInstance, int nCmdShow, LPCWSTR windowName, LPCWSTR windowClass) {
    HRESULT hr = S_OK;

    _window = new Window(hInstance, nCmdShow, windowName, windowClass);

    if (FAILED(_window->Initialise())) {
        return E_FAIL;
    }

    _dxDevice = new DxDevice();
    if (FAILED(_dxDevice->InitDevice())) {
        Destroy();

        return E_FAIL;
    }

    _d3dDevice = _dxDevice->GetDevice();
	_immediateContext = _dxDevice->GetDeviceContext();

    // Create a render target view
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    if (FAILED(hr))
        return hr;

    hr = _d3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &_renderTargetView);
    pBackBuffer->Release();

    if (FAILED(hr))
        return hr;


    // Setup Depth Buffer
    D3D11_TEXTURE2D_DESC depthStencilDesc; // Descriptor is the way we create Texture2Ds in Dx11


    UINT windowWidth = _window->GetWidth();
    UINT windowHeight = _window->GetHeight();

    depthStencilDesc.Width = windowWidth;
    depthStencilDesc.Height = windowHeight;

    depthStencilDesc.MipLevels = 1; // TODO: Check if this is mipmapping or not and is on basic level.
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    _d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, &_depthStencilBuffer);
    if (_depthStencilBuffer) {
        _d3dDevice->CreateDepthStencilView(_depthStencilBuffer, nullptr, &_depthStencilView);
    }

    _immediateContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)windowWidth;
    vp.Height = (FLOAT)windowHeight;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    _immediateContext->RSSetViewports(1, &vp);

    //InitShadersAndInputLayout();

    //InitVertexBuffer();

    //InitIndexBuffer();

    // Set primitive topology
    _immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Create the constant buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(ConstantBuffer);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    hr = _d3dDevice->CreateBuffer(&bd, nullptr, &_constantBuffer);

    if (FAILED(hr))
        return hr;


    D3D11_RASTERIZER_DESC wfdesc;
    ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
    wfdesc.FillMode = D3D11_FILL_WIREFRAME;
    wfdesc.CullMode = D3D11_CULL_NONE;
    hr = _d3dDevice->CreateRasterizerState(&wfdesc, &_rasterState);

    if (FAILED(hr))
        return hr;

    return S_OK;
}