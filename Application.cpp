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

Application::Application() {
    _time = nullptr;
	_hInst = nullptr;
    _graphicManager = nullptr;
    _objectManager = nullptr;
    _textureManager = nullptr;

    _spinningObject = nullptr;
}

Application::~Application() {
	Cleanup();
}

HRESULT Application::Initialise(HINSTANCE hInstance, int nCmdShow) {
    if (FAILED(SetupManagers(hInstance, nCmdShow))) {
        return E_FAIL;
    }

    SetupObjects();

	return S_OK;
}


void Application::Cleanup() {
    if (_graphicManager) delete _graphicManager;
    if (_textureManager) delete _textureManager;
    if (_objectManager) delete _objectManager;
    if (_spinningObject) delete _spinningObject;
    if (_time) delete _time;
}

void Application::Update() {
    _time->StartFrame();
    _time->UpdateFPS();
    _inputManager->Update();
    _objectManager->Update();

    _time->EndFrame();

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

HRESULT Application::SetupManagers(HINSTANCE hInstance, int nCmdShow) {
    _time = new Time();
    _inputManager = new InputManager();
    _inputManager->SetTime(_time);

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

    _objectManager->SetTime(_time);

    _graphicManager->SetObjectManager(_objectManager);
    _graphicManager->SetTime(_time);
    _graphicManager->SetInputManager(_inputManager);

    return S_OK;
}

void Application::SetupObjects() {
    MeshData* model = new MeshData(OBJLoader::Load("Hercules.obj", _graphicManager->GetDevice()->GetDevice()));

    _spinningObject = new SpinningObject(XMFLOAT3(8, 0, 5));

    _spinningObject->LoadModel(model);

    Texture* texture = new Texture(L"Pain");
    _spinningObject->LoadTexture(texture->LoadTexture(L"Hercules_COLOR.dds", _graphicManager->GetDevice()->GetDevice()));

    Shader* shader = new Shader();
    shader->Initialise(L"DX11 Framework.fx", _graphicManager->GetDevice()->GetDevice(), _graphicManager->GetDevice()->GetDeviceContext());
    _spinningObject->LoadShader(shader);

    _objectManager->AddObject(_spinningObject);
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