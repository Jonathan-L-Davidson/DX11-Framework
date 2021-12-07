#include "Window.h"
using namespace DirectX;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)     {
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

Window::Window(HINSTANCE hInstance, int nCmdShow, LPCWSTR windowName, LPCWSTR windowClass) {
    _hInst = hInstance;
    _cmdShow = nCmdShow;
    _windowName = windowName;
    _windowClass = windowClass;

    _width = 0; _height = 0;
    _hWnd = nullptr;

    XMStoreFloat4x4(&_view, XMMatrixIdentity());
    XMStoreFloat4x4(&_projection, XMMatrixIdentity());

    _camera = nullptr;
}

void Window::UpdateViewMatrix() {
    XMStoreFloat4x4(&_view, XMMatrixLookAtLH(_camera->Eye, _camera->At, _camera->Up));
}

HRESULT Window::Initialise() {
    _camera = new Camera();

    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = _hInst;
    wcex.hIcon = LoadIcon(_hInst, (LPCTSTR)IDI_TUTORIAL1);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = _windowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);
    if (!RegisterClassEx(&wcex))
        return E_FAIL;

    // Create window
    AdjustWindowRect(&_size, WS_OVERLAPPEDWINDOW, FALSE);
    _hWnd = CreateWindow(_windowClass, _windowName, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, _size.right - _size.left, _size.bottom - _size.top, nullptr, nullptr, _hInst,
        nullptr);

    if (!_hWnd)
        return E_FAIL;

    ShowWind();
    
    _width = _size.right - _size.left;
    _height = _size.bottom - _size.top;

    // Initialize the projection matrix
    XMStoreFloat4x4(&_projection, XMMatrixPerspectiveFovLH(XM_PIDIV2, _width / (FLOAT)_height, 0.01f, 100.0f));


    return S_OK;
}

void Window::Destroy() {
    if (_camera) _camera = nullptr;

}